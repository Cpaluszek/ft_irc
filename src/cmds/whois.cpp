/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:47:12 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:47:12 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

std::string getChannelsList(const Client *client, const std::string &targetNick, const Client *target) {
	std::string channelsList;
	Client::channelMap channels = target->getChannels();
	Client::channelMapIt it;
	if (channels.empty()) {
		channelsList = "*";
	}
	for (it = channels.begin(); it != channels.end(); it++) {
		bool isOperator = target->hasMode('o');
		bool isClientInChannel = it->second->isClientConnected(client->nickName);
		if (isOperator || client->nickName == targetNick || isClientInChannel) {
			if (it  != channels.begin()) {
				channelsList += " ";
			}
			channelsList += it->second->getPrefix(targetNick);
			channelsList += it->second->getName();
		}
	}
	return channelsList;
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#ping-message)
void whoisCmd(Client *client, const Request &request, Server *server) {
	if (request.args.empty()) {
		Server::sendToClient(client->socketFd, ERR_NONICKNAMEGIVEN(client->nickName));
		return ;
	}
	std::string targetNick = request.args[0];
	if (server->getClientByNick(targetNick) == NULL) {
		Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, targetNick));
		return ;
	}
	Client *target = server->getClientByNick(targetNick);
	if (target == NULL) {
		std::cerr << RED << "Error: whois: target is NULL" << RESET << std::endl;
		return ;
	}
	if (target->hasMode('r')) {
		Server::sendToClient(client->socketFd, RPL_WHOISREGNICK(client->nickName, targetNick));
	}
	if (target->isAway) {
		Server::sendToClient(client->socketFd, RPL_AWAY(client->nickName, target->nickName, target->awayMessage));
	}
	// Client info
	Server::sendToClient(client->socketFd, RPL_WHOISUSER(client->nickName, targetNick, target->userName, target->realName));
	// Server info
	Server::sendToClient(client->socketFd, RPL_WHOISSERVER(client->nickName, target->nickName, SERVER_INFO));
	// is the client operator
	bool isTargetOperator = target->hasMode('o');
	bool isClientOperator = client->hasMode('o');
	if (isTargetOperator) {
		Server::sendToClient(client->socketFd, RPL_WHOISOPERATOR(client->nickName, target->nickName));
	}
	// client channels list
	std::string channelsList = getChannelsList(client, targetNick, target);
	Server::sendToClient(client->socketFd, RPL_WHOISCHANNEL(client->nickName, targetNick, channelsList));

	if (isClientOperator || client->nickName == targetNick) {
		Server::sendToClient(client->socketFd, RPL_WHOISHOST(client->nickName, targetNick));
		Server::sendToClient(client->socketFd, RPL_WHOISMODES(client->nickName, targetNick, target->getMode()));
	}
	Server::sendToClient(client->socketFd, RPL_ENDOFWHOIS(client->nickName, targetNick));
}
