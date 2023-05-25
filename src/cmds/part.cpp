/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlitaudo <jlitaudo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 14:44:02 by jlitaudo          #+#    #+#             */
/*   Updated: 2023/05/17 15:18:59 by jlitaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

void	quitChannel(Client *client, Channel *channel);

void	partCmd(Client *client, const Request &request, Server *server)
{
	if (request.args.empty()){
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, request.command));
		return ;
	}
	std::vector<std::string>::iterator it;
	for (it = request.args.begin(); it != request.args.end(); it++)
	{
		std::map<std::string, Channel*>::iterator channelIt = server->getChannelByName(*it);
		if (channelIt == server->getChannelEnd())
			Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, *it));
		else if (client->isOnChannel(*it) == false)
			Server::sendToClient(client->socketFd, ERR_NOTONCHANNEL(client->nickName, *it));
		else
			quitChannel(client, channelIt->second);
	}

}

void	quitChannel(Client *client, Channel *channel)
{
	client->eraseChannel(channel->name);
	channel->eraseClient(client->nickName);
	Server::sendToClient(client->socketFd, ("You have quit channel " + channel->name + "\n"));
	//message sur le channel
}