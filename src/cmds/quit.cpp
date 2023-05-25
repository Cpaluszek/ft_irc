/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlitaudo <jlitaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:47:34 by jlitaudo          #+#    #+#             */
/*   Updated: 2023/05/25 14:51:35 by jlitaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"
#include "Channel.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#quit-message)
void quitCmd(Client *client, const Request &request, Server *server) {
	(void) request;

	client->quit();
	Server::sendToClient(client->socketFd, ERR_MSG(std::string("Disconnected from server")));
	server->disconnectClient(client->socketFd);

}
