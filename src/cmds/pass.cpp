/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:45:00 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:45:00 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#pass-message)
void passCmd(Client *client, const Request &request, Server *server) {
	if (client->hasPassword) {
		return ;
	}
	if (client->hasMode('r')) {
		Server::sendToClient(client->socketFd, ERR_ALREADYREGISTERED(client->nickName));
	}
	else if (request.args.empty() || request.args[0].length() == 0) {
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, std::string("PASS")));
	}
	else if (request.args.size() == 1 && request.args[0] == server->getPassword()) {
		client->hasPassword = true;
	}
	else {
		Server::sendToClient(client->socketFd, ERR_PASSWDMISMATCH(std::string("Client")));
		server->disconnectClient(client);
	}
}
