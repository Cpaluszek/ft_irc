#include "commands.hpp"
#include "Channel.hpp"

void connectClientToChannel(Client *client, Channel *channel) {
	// Todo: check if banned
	// Todo: check if channel is full
	// Todo: check invitation required ?

	// Success
	client->addChannel(channel);
	channel->addClient(client);

	Channel::mapClients clients = channel->getClients();
	std::string names;

	// Send JOIN to all clients in the channel
	for (Channel::mapClientsIt it = clients.begin(); it != clients.end(); it++) {
		Server::sendToClient(it->second.client->socketFd, RPL_CMD(client->nickName, client->userName, "JOIN", channel->name));
		if (it != clients.begin()) {
			names.append(" ");
		}
		names.append(channel->getPrefix(it->first) + it->second.client->nickName);
	}

	// Send channel _topic
	if (channel->getTopic().length() > 0) {
		Server::sendToClient(client->socketFd, RPL_TOPIC(client->nickName, channel->name, channel->getTopic()));
		Server::sendToClient(client->socketFd, RPL_TOPICWHOTIME(client->nickName, channel->name, channel->getTopicUser(), channel->getTopicTime()));
	}

	// Send list of names
	Server::sendToClient(client->socketFd, RPL_NAMREPLY(client->nickName, channel->symbol, channel->name, names));
	Server::sendToClient(client->socketFd, RPL_ENDOFNAMES(client->nickName, channel->name));
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#join-message)
void joinCmd(Client *client, const Request &request, Server *server) {
	if (request.args.empty()) {
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, "JOIN"));
	}
	if (request.args[0] == "0") {
		// Todo: leave all channels
		std::cout << "Leave all channels" << std::endl;
		return ;
	}
	std::vector<std::string> channels = Utils::split(request.args[0], ",");
	std::vector<std::string> keys;
	if (request.args.size() > 1) {
		keys = Utils::split(request.args[0], ",");
	}

	std::vector<std::string>::iterator nameIt;
	std::vector<std::string>::iterator keyIt = keys.begin();
	for (nameIt = channels.begin(); nameIt != channels.end(); nameIt++) {
		// check if the channel name is too long
		if (nameIt->length() > CHANNELLEN) {
			Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, *nameIt));
			continue ;
		}
		// check for banned chars
		if (nameIt->find_first_of(" ,\a") != std::string::npos) {
			Server::sendToClient(client->socketFd, ERR_BADCHANMASK(*nameIt));
			continue ;
		}
		// check 1st char == # || &
		if ((*nameIt)[0] != '#' && (*nameIt)[0] != '&') {
			Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, *nameIt));
			continue ;
		}
		// If the client follows to many channels
		if (client->getNumberOfChannelsJoined() >= CHANLIMIT) {
			Server::sendToClient(client->socketFd, ERR_TOOMANYCHANNELS(client->nickName, *nameIt));
			continue ;
		}

		Server::channelIt existingChannelIt = server->getChannelByName(*nameIt);
		if (existingChannelIt == server->getChannelEnd()) {
			Channel *newChannel = new Channel(*nameIt, client, server);

			server->addChannel(newChannel);
			client->addChannel(newChannel);

			Server::sendToClient(client->socketFd, RPL_CMD(client->nickName, client->userName, "JOIN", newChannel->name));
			Server::sendToClient(client->socketFd, RPL_NAMREPLY(client->nickName, newChannel->symbol,
																newChannel->name, newChannel->getPrefix(client->nickName) + client->nickName));
			Server::sendToClient(client->socketFd, RPL_ENDOFNAMES(client->nickName, newChannel->name));
			continue ;
		}
		else {
			Channel *currentChannel = existingChannelIt->second;
			if (currentChannel->getMods().find_first_of('k') != std::string::npos) {
				if (keyIt != keys.end() && currentChannel->getKey() != *keyIt) {
					Server::sendToClient(client->socketFd, ERR_BADCHANNELKEY(client->nickName, currentChannel->name));
				}
				else {
					connectClientToChannel(client, currentChannel);
				}
			}
			else {
				connectClientToChannel(client, currentChannel);
			}
		}
		if (keyIt != keys.end()) {
			keyIt++;
		}
	}
}
