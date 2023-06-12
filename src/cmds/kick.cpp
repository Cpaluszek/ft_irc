/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:44:36 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/12 13:30:43 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

void kickFromChannel(Client *client, Client *user, Channel *channel, const std::string& reason);

void	kickCmd(Client *client, const Request &request, Server *server)
{
    if (request.args.size() < 2) {
        Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, request.command));
        return ;
    }
	std::string channelName = request.args[0];
    Channel *channel = server->getChannelByName(channelName);
    if (channel == NULL) {
        Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, channelName));
        return ;
    }
    if (!client->isOnChannel(channelName)) {
        Server::sendToClient(client->socketFd, ERR_NOTONCHANNEL(client->nickName, channelName));
        return ;
    }
	if (!channel->isClientOperator(client->nickName)) {
		Server::sendToClient(client->socketFd, ERR_CHANOPRIVSNEEDED(client->nickName, channelName));
		return ;
	}
	std::string targetNickName = request.args[1];
	std::string reason = request.args.size() > 2 ? request.args[2] : "";

	Client *user = server->getClientByNick(targetNickName);
	if (!channel->isClientConnected(targetNickName))
		Server::sendToClient(client->socketFd, ERR_USERNOTINCHANNEL(client->nickName, targetNickName, channel->getName()));
	else
		kickFromChannel(client, user, channel, reason);
}

void kickFromChannel(Client *client, Client *user, Channel *channel, const std::string& reason)
{
    Server::sendToClient(user->socketFd, RPL_CMD(user->nickName, user->userName, "KICK", channel->getName() + " " + client->nickName + " " + reason));
	channel->sendToAllClients(
			RPL_CMD(client->nickName, client->userName, "KICK", channel->getName() + " " + user->nickName + " " + reason));
    user->eraseChannel(channel->getName());
    channel->eraseClient(user->nickName);
}
