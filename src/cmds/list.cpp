/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:44:39 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/12 11:43:48 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

void sendChannelInformation(Client *client, Channel *channel) {
	std::string channelName = channel->getName();
	size_t clientCount = channel->getClientCount();
	std::stringstream s;
	s << clientCount;
	std::string clientCountStr = s.str();
	std::string channelTopic = channel->getTopic();
	Server::sendToClient(client->socketFd, RPL_LIST(client->nickName, channelName, clientCountStr, channelTopic));
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#list-message)
void listCmd(Client *client, const Request &request, Server *server) {
	Server::channelMap channels = server->getChannels();
	Server::channelIt it;
	if (request.args.empty()) {
		Server::sendToClient(client->socketFd, RPL_LISTSTART(client->nickName));
		for (it = channels.begin(); it != channels.end(); it++) {
			// If the channel is not secret 's'
			if (it->second->getMode().find('s') == std::string::npos) {
				sendChannelInformation(client, it->second);
			}
		}
		Server::sendToClient(client->socketFd, RPL_LISTEND(client->nickName));
		return ;
	}
	Server::vecStr targetChannels = Utils::split(request.args[0], ",", false);
	Server::vecStrIt itNames;
	for (itNames = targetChannels.begin(); itNames != targetChannels.end(); itNames++) {
		Channel *channel = server->getChannelByName(*itNames);
		if (channel == NULL)
			std::cerr << "No such channel: " << *itNames << std::endl;
		else
			sendChannelInformation(client, channel);
	}
}
