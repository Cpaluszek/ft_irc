#include "commands.hpp"
#include "Channel.hpp"

// Todo: test limit and invite mode
void connectClientToChannel(Client *client, Channel *channel) {
	// Check if invitation is required
	if (channel->hasMode('i') && !channel->isInvited(client->nickName)) {
		Server::sendToClient(client->socketFd, ERR_INVITEONLYCHAN(client->nickName, channel->getName()));
		return ;
	}

	// Check if the channel as reached limit
	if (channel->hasMode('l') && channel->getClientLimit() <= channel->getClientCount()) {
		Server::sendToClient(client->socketFd, ERR_CHANNELISFULL(client->nickName, channel->getName()));
		return ;
	}

	// Success
	client->addChannel(channel);
	channel->addClient(client);

	Channel::mapClients clients = channel->getClients();
	std::string names;

	// Send JOIN to all clients in the channel
	for (Channel::mapClientsIt it = clients.begin(); it != clients.end(); it++) {
		Server::sendToClient(it->second.client->socketFd, RPL_CMD(client->nickName, client->userName, "JOIN", channel->getName()));
		if (it != clients.begin()) {
			names.append(" ");
		}
		names.append(channel->getPrefix(it->first) + it->second.client->nickName);
	}

	// Send channel _topic
	if (channel->getTopic().length() > 0) {
		Server::sendToClient(client->socketFd, RPL_TOPIC(client->nickName, channel->getName(), channel->getTopic()));
		Server::sendToClient(client->socketFd, RPL_TOPICWHOTIME(client->nickName, channel->getName(), channel->getTopicUser(), channel->getTopicTime()));
	}

	// Send list of names
	Server::sendToClient(client->socketFd, RPL_NAMREPLY(client->nickName, channel->getSymbol(), channel->getName(), names));
	Server::sendToClient(client->socketFd, RPL_ENDOFNAMES(client->nickName, channel->getName()));
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#join-message)
void joinCmd(Client *client, const Request &request, Server *server) {
	if (request.args.empty()) {
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, "JOIN"));
		return ;
	}
	if (request.args[0] == "0") {
		client->leaveAllChannels();
		return ;
	}
	Server::vecStr channels = Utils::split(request.args[0], ",", false);
	Server::vecStr keys;
	if (request.args.size() > 1) {
		keys = Utils::split(request.args[1], ",", false);
	}

	Server::vecStrIt nameIt;
	Server::vecStrIt keyIt = keys.begin();
	for (nameIt = channels.begin(); nameIt != channels.end(); nameIt++) {
		// check if the channel _name is too long
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

		Channel *existingChannel = server->getChannelByName(*nameIt);
		if (existingChannel == NULL) {
			Channel *newChannel = new Channel(*nameIt, client, server);

			if (keyIt != keys.end()) {
				std::cout << RED << *nameIt << " key: " << *keyIt << std::endl;
				newChannel->setKey(*keyIt);
				newChannel->addMode('k');
			}
			server->addChannel(newChannel);
			client->addChannel(newChannel);

			Server::sendToClient(client->socketFd, RPL_CMD(client->nickName, client->userName, "JOIN", newChannel->getName()));
			Server::sendToClient(client->socketFd, RPL_NAMREPLY(client->nickName, newChannel->getSymbol(),
																newChannel->getName(), newChannel->getPrefix(client->nickName) + client->nickName));
			Server::sendToClient(client->socketFd, RPL_ENDOFNAMES(client->nickName, newChannel->getName()));
			continue ;
		}
		else if (!existingChannel->isClientConnected(client->nickName)) {
			if (existingChannel->hasMode('k') && (keyIt == keys.end() || existingChannel->getKey() != *keyIt)) {
				Server::sendToClient(client->socketFd, ERR_BADCHANNELKEY(client->nickName, existingChannel->getName()));
			}
			else {
				connectClientToChannel(client, existingChannel);
			}
		}
		if (keyIt != keys.end()) {
			keyIt++;
		}
	}
}
