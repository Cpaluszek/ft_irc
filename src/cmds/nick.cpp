/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:44:53 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:44:53 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

bool containsInvalidChars(std::string nick) {
	return (nick.find_first_of(" ,*?!@.") != std::string::npos ||
	 nick[0] == '$' || nick[0] == ':' || nick[0] == '#' || nick[0] == '&');
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#nick-message)
void nickCmd(Client *client, const Request &request, Server *server) {
	if (!client->hasPassword) {
		Server::sendToClient(client->socketFd, ERR_MSG(std::string("PASS is needed")));
		return ;
	}
	// No nickname given
	std::string nick = request.args[0];
	if (request.args.empty() || request.args[0].length() == 0) {
		Server::sendToClient(client->socketFd, ERR_NONICKNAMEGIVEN(std::string("Client")));
	}
	// Invalid characters
	else if (containsInvalidChars(nick)) {
		Server::sendToClient(client->socketFd, ERR_ERRONEUSNICKNAME(nick));
	}
	// Nickname already used
	else if (server->isNickAlreadyUsed(*client, nick)) {
		Server::sendToClient(client->socketFd, ERR_NICKNAMEINUSE(nick));
	}
	// Success
	else {
		client->updateNickname(nick);
		if (!client->hasMode('r') && client->userName.length() != 0) {  // First nickname input
			client->addMode('r');
			server->sendWelcome(client);
		}
		else { // Update nickname
			Server::sendToClient(client->socketFd, RPL_NICK(client->previousNickname, nick, client->userName));
			Client::channelMap channels = client->getChannels();
			Client::channelMapIt it;
			for (it = channels.begin(); it != channels.end(); it++) {
				it->second->updateClient(client->previousNickname, client->nickName);

				// Alert all users in the channel that the nickname has changed
				Channel::mapClients channelClients = it->second->getClients();
				Channel::mapClientsIt channelClientIt;
				for (channelClientIt = channelClients.begin(); channelClientIt != channelClients.end(); channelClientIt++) {
					Client *current = channelClientIt->second.client;
					if (current->nickName != client->nickName) {
						Server::sendToClient(current->socketFd, RPL_NICK(client->previousNickname, client->nickName, client->userName));
					}
				}
			}
		}
	}
}
