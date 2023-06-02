#include "commands.hpp"

void getChannelNames(Channel *channel, Client *client) {
	Channel::mapClients clients = channel->getClients();
	Channel::mapClientsIt it;
	std::string names;
	for (it = clients.begin(); it != clients.end(); it++) {
		if (it != clients.begin()) {
			names.append(" ");
		}
		names.append(channel->getPrefix(it->first) + it->second.client->nickName);
	}
	// Send list of names
	Server::sendToClient(client->socketFd, RPL_NAMREPLY(client->nickName, channel->symbol, channel->name, names));
	Server::sendToClient(client->socketFd, RPL_ENDOFNAMES(client->nickName, channel->name));
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#names-message)
void namesCmd(Client *client, const Request &request, Server *server) {
	// List all users
	if (request.args.empty() || request.args[0] == SERVER_NAME) {
		std::string names;
		// Users currently connected to a channel
		Server::channelMap channels = server->getChannels();
		Server::channelIt channelIt;
		for (channelIt = channels.begin(); channelIt != channels.end(); channelIt++) {
			getChannelNames(channelIt->second, client);
		}

		// Add all other users
		Server::clientMap clients = server->getClients();
		Server::clientIt clientIt;
		bool endOfNamesNeeded = false;
		for (clientIt = clients.begin(); clientIt != clients.end(); clientIt++) {
			bool isInvisible = clientIt->second->hasMode('i');
			bool hasJoinedAChannel = !clientIt->second->getChannels().empty();
			if (isInvisible || hasJoinedAChannel) {
				continue;
			}
			if (endOfNamesNeeded) {
				names.append(" ");
			}
			names.append(clientIt->second->nickName);
			endOfNamesNeeded = true;

		}
		if (endOfNamesNeeded) {
			Server::sendToClient(client->socketFd, RPL_NAMREPLY(client->nickName, "*", SERVER_NAME, names));
			Server::sendToClient(client->socketFd, RPL_ENDOFNAMES(client->nickName, SERVER_NAME));
		}
		return ;
	}
	// Get the channel names input
	Server::vecStr channelNames = Utils::split(request.args[0], ",", false);
	Server::vecStrIt it;
	for (it = channelNames.begin(); it != channelNames.end(); it++) {
		Channel *channel = server->getChannelByName(*it);
		if (channel != NULL) {
			getChannelNames(channel, client);
		}
		else {
			Server::sendToClient(client->socketFd, RPL_ENDOFNAMES(client->nickName, *it));
		}
	}
}
