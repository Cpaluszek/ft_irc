#include "commands.hpp"

void whoWithoutMask(const Client *client, Server *server) {
	Server::clientIt it;
	for (it = server->getClientBeginIt(); it != server->getClientEndIt(); it++) {
		std::string channelName;
		std::string flags = "H";
		Client *currentClient = it->second;

		Client::channelMap channels = it->second->getChannels();
		if (channels.empty()) {
			channelName = "*";
		}
		else {
			channelName = channels.begin()->second->name;
			// Todo: check operator grade for it->second
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
		if (operatorOnly && currentClient->getMode().find('o') == std::string::npos) {
			continue ;
		}
		bool isVisible = currentClient->getMode().find('i') == std::string::npos;
		if (isVisible || isClientInChannel) {
			std::string flags = "H";
			// Todo: check operator grade for client
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
		if (operatorOnly && currentClient->getMode().find('o') == std::string::npos) {
			continue ;
		}

		// MASK
		if (mask == currentClient->nickName || mask == currentClient->userName || mask == currentClient->realName ||\
			mask == SERVER_NAME || mask == SERVER_IP) {
			std::string channelName;
			std::string flags = "H";
		 	Client::channelMap channels = currentClient->getChannels();
			 if (channels.empty()) {
				 channelName = "*";
			 }
			 else {
				 channelName = channels.begin()->second->name;
				 // Todo: check operator grade for client
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
// Note: what is flag H
void whoCmd(Client *client, const Request &request, Server *server) {
	std::string mask = request.args.empty() ? "" : request.args[0];

	if (request.args.empty()) {
		// List visible clients or clients sharing a channel
		whoWithoutMask(client, server);
	}
	else {
		bool operatorOnly = request.args.size() >= 2 && request.args[1] == "o";
		// Find the channel
		Server::channelIt channelIt = server->getChannelByName(request.args[0]);
		if (channelIt != server->getChannelEnd()) {
			// List all users in the channel
			whoChannel(client, channelIt->second, operatorOnly);
		}
		else {
			// If 1st arg is not a channel -> list all users checking the mask
			whoMask(client, server, mask, operatorOnly);
		}
	}
	// Send RPL_ENDOFWHO
	Server::sendToClient(client->socketFd, RPL_ENDOFWHO(client->nickName, mask));
}
