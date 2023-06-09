/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:44:47 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:44:47 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"
#include <fstream>

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#motd-message)
void motdCmd(Client *client, const Request &request, Server *server) {
	(void) request;
	(void) server;
	std::ifstream file;
	file.open(MOTD_FILE);
	if (!file.is_open()) {
		Server::sendToClient(client->socketFd, ERR_NOMOTD(client->nickName));
		return ;
	}
	std::string buffer;
	std::getline(file, buffer, '\0');
	if (buffer.empty()) {
		Server::sendToClient(client->socketFd, ERR_NOMOTD(client->nickName));
	}
	else {
		Server::sendToClient(client->socketFd, RPL_MOTDSTART(client->nickName));
		Server::sendToClient(client->socketFd, RPL_MOTD(client->nickName, buffer));
		Server::sendToClient(client->socketFd, RPL_ENDOFMOTD(client->nickName));
	}
	file.close();
}
