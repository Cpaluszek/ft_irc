/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:44:29 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/12 10:51:35 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#invite-message)
void inviteCmd(Client *client, const Request &request, Server *server) {
	if (request.args.empty() || request.args.size() < 2) {
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, request.command));
		return ;
	}

	std::string targetNick = request.args[0];
	std::string targetChannel = request.args[1];

	// Check the channel existence - ERR_NOSUCHCHANNEL
	Channel *channel = server->getChannelByName(targetChannel);
	if (channel == NULL) {
		Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, targetChannel));
		return ;
	}

	// Check the presence on targetChannel - ERR_NOTONCHANNEL
	if (!channel->isClientConnected(client->nickName)) {
		Server::sendToClient(client->socketFd, ERR_NOTONCHANNEL(client->nickName, targetChannel));
		return ;
	}

	// Check if the channel has mode 'i' and user is not a channel operator - ERR_CHANOPRIVSNEEDED
	channelUser *chanUser = channel->getChannelUserByNick(client->nickName);
	if (channel->hasMode('i') && chanUser->userMode.find('o') == std::string::npos) {
		Server::sendToClient(client->socketFd, ERR_CHANOPRIVSNEEDED(client->nickName, targetChannel));
		return ;
	}
	// Check if the target user is connected to the server
	Client *targetClient = server->getClientByNick(targetNick);
	if (targetClient == NULL) {
		Server::sendToClient(client->socketFd, ERR_MSG(std::string("User (" + targetNick + ") not found")));
		return ;
	}

	// Check if the user is already connected on the channel - ERR_USERONCHANNEL
	if (channel->isClientConnected(targetNick)) {
		Server::sendToClient(client->socketFd, ERR_USERONCHANNEL(client->nickName, targetNick, targetChannel));
		return ;
	}
	Server::sendToClient(targetClient->socketFd, RPL_INVITE(client->nickName, client->userName, targetChannel, targetNick));
	Server::sendToClient(client->socketFd, RPL_INVITING(client->nickName, targetNick, targetChannel));
	channel->addInvite(targetNick);
}
