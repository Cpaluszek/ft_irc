/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:45:17 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/12 10:39:15 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#user-message)
/**
 * Command: USER
 * 		Parameters: <username> 0 * <realname>
 *
 * Example:
 * USER guest 0 * :Ronnie Reagan
 * 	- user get registered with username "guest" and realname "Ronnie Reagan"
 */
void userCmd(Client *client, const Request &request, Server *server) {
	if (!client->hasPassword) {
		Server::sendToClient(client->socketFd, ERR_MSG(std::string("PASS is needed")));
		return;
	}
	if (request.args.empty() || request.args.size() < 3){
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, std::string("USER")));
		return;
	}
	if (request.args[1] != "0" || request.args[2] != "*") {
		Server::sendToClient(client->socketFd, ERR_MSG(std::string("Format: USER <username> 0 * <realname>")));
		return;
	}
	if (client->hasMode('r')) {
		Server::sendToClient(client->socketFd, ERR_ALREADYREGISTERED(client->nickName));
		return ;
	}
	client->userName = request.args[0];
	if (request.args.size() > 3) {
		client->realName = request.args[3];
	}
	if (!client->nickName.empty()) {
		client->addMode('r');
		server->sendWelcome(client);
	}
}
