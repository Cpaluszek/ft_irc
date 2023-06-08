/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:44:57 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:44:57 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

void leaveChannel(Client *client, Channel *channel, const std::string& reason);

void	partCmd(Client *client, const Request &request, Server *server) {
	if (request.args.empty()){
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, request.command));
		return ;
	}
	Server::vecStr targetChannels = Utils::split(request.args[0], ",", false);
	std::string reason = request.args.size() > 1 ? request.args[1] : "No reason";
	for (Server::vecStrIt it = targetChannels.begin(); it != targetChannels.end(); it++) {
		std::string targetChannel = *it;
		Channel *channel = server->getChannelByName(targetChannel);
		if (channel == NULL)
			Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, targetChannel));
		else if (!client->isOnChannel(targetChannel))
			Server::sendToClient(client->socketFd, ERR_NOTONCHANNEL(client->nickName, targetChannel));
		else
			leaveChannel(client, channel, reason);
	}

}

void leaveChannel(Client *client, Channel *channel, const std::string& reason) {
	channel->sendToAllClients(
			RPL_CMD(client->nickName, client->userName, "PART", (channel->getName() + " " + reason)));
	client->eraseChannel(channel->getName());
	channel->eraseClient(client->nickName);
}
