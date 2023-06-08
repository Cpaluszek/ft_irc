/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:44:26 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:44:26 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#away-message)
void awayCmd(Client *client, const Request &request, Server *server) {
	(void) server;
	if (request.args.empty() || request.args[0].empty()) {
		if (client->isAway) {
			client->isAway = false;
			Server::sendToClient(client->socketFd, RPL_UNAWAY(client->nickName));
		}
	}
	else if (!client->isAway){
		client->awayMessage = request.args[0];
		client->isAway = true;
		Server::sendToClient(client->socketFd, RPL_NOWAWAY(client->nickName));
	}
}
