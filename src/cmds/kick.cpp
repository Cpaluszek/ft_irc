//
// Created by jlitaudo on 5/31/23.
//

#include "commands.hpp"

void    kickFromChannel(Client *client, Client *user, Channel *channel);

void	kickCmd(Client *client, const Request &request, Server *server)
{
    std::cout << request << std::endl;
    if (request.args.size() < 2){
        Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, request.command));
        return ;
    }
    std::vector<std::string>::const_iterator it = request.args.begin();
//    it++;
    Channel *channel = server->getChannelByName(*it);
    if (channel == NULL)
    {
        Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, *it));
        return ;
    }
    else if (!client->isOnChannel(*it))
    {
        Server::sendToClient(client->socketFd, ERR_NOTONCHANNEL(client->nickName, *it));
        return ;
    }
    Client  *user;
    it++;
    for (; it != request.args.end(); it++)
    {
        user = server->getClientByNick(*it);
        if (!channel->isClientConnected(*it))
            Server::sendToClient(client->socketFd, ERR_USERNOTINCHANNEL(client->nickName, *it, channel->getName()));
        else
            kickFromChannel(client, user, channel);
    }
}

void    kickFromChannel(Client *client, Client *user, Channel *channel)
{
    Server::sendToClient(user->socketFd, RPL_CMD(user->nickName, user->userName, "KICK", ("You have been kicked from " + channel->getName() + " by " + client->nickName)));
    user->eraseChannel(channel->getName());
    channel->eraseClient(user->nickName);
	channel->sendToAllClients(
			RPL_CMD(client->nickName, client->userName, "KICK", (channel->getName() + " " + user->nickName)));
}

