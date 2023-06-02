#include "commands.hpp"

void whoWithoutMask(const Client *client, Server *server) {
	Server::clientIt it;
	for (it = server->getClientBeginIt(); it != server->getClientEndIt(); it++) {
		std::string channelName;
		Client *currentClient = it->second;
		std::string flags = currentClient->isAway ? "G" : "H";

		Client::channelMap channels = it->second->getChannels();
		if (channels.empty()) {
			channelName = "*";
		}
		else {
			channelName = channels.begin()->second->name;
			flags.append(channels.begin()->second->getPrefix(currentClient->nickName));
		}
		Server::sendToClient(client->socketFd, RPL_WHOREPLY(client->nickName, currentClient->nickName, currentClient->userName \
			, currentClient->realName, channelName, flags));
	}
}


void whoChannel(const Client *client, Channel *channel, bool operatorOnly) {
	Channel::mapClients clients = channel->getClients();
	Channel::mapClientsIt it;

	bool isClientInChannel = channel->isClientConnected(client->nickName);
	for (it = clients.begin(); it != clients.end(); it++) {
		Client *currentClient = it->second.client;
		if (operatorOnly && currentClient->hasMode('o')) {
			continue ;
		}
		bool isVisible = !currentClient->hasMode('i');
		if (isVisible || isClientInChannel) {
			std::string flags = currentClient->isAway ? "G" : "H";
			flags.append(it->second.prefix);
			Server::sendToClient(client->socketFd, RPL_WHOREPLY(client->nickName, currentClient->nickName, currentClient->userName \
					, currentClient->realName, channel->name, flags));
		}
	}
}

void whoMask(const Client *client, Server *server, const std::string& mask, bool operatorOnly) {
	// Get all clients from the server
	Server::clientIt it;
	for (it = server->getClientBeginIt(); it != server->getClientEndIt(); it++) {
		Client *currentClient = it->second;
		if (operatorOnly && currentClient->hasMode('o')) {
			continue ;
		}

		// MASK
		if (mask == currentClient->nickName || mask == currentClient->userName || mask == currentClient->realName ||\
			mask == SERVER_NAME || mask == LOCAL_HOST_IP) {
			std::string channelName;
			std::string flags = currentClient->isAway ? "G" : "H";
		 	Client::channelMap channels = currentClient->getChannels();
			 if (channels.empty()) {
				 channelName = "*";
			 }
			 else {
				 channelName = channels.begin()->second->name;
				 flags.append(channels.begin()->second->getPrefix(currentClient->nickName));
			 }
			 Server::sendToClient(client->socketFd, RPL_WHOREPLY(client->nickName, currentClient->nickName, currentClient->userName, \
			 		currentClient->realName, channelName, flags));
		}
	}
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#who-message)
//  WHO emersion        ; request information on user "emersion"
//  WHO #ircv3          ; list users in the "#ircv3" channel
void whoCmd(Client *client, const Request &request, Server *server) {
	std::string mask = request.args.empty() ? "" : request.args[0];

	if (request.args.empty()) {
		// List visible clients or clients sharing a channel
		whoWithoutMask(client, server);
	}
	else {
		bool operatorOnly = request.args.size() >= 2 && request.args[1] == "o";
		// Find the channel
		Channel *channel = server->getChannelByName(request.args[0]);
		if (channel != NULL) {
			whoChannel(client, channel, operatorOnly);
		}
		else {
			whoMask(client, server, mask, operatorOnly);
		}
	}
	Server::sendToClient(client->socketFd, RPL_ENDOFWHO(client->nickName, mask));
}
