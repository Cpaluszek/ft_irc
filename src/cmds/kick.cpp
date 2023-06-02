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
    else if (client->isOnChannel(*it) == false)
    {
        Server::sendToClient(client->socketFd, ERR_NOTONCHANNEL(client->nickName, *it));
        return ;
    }
    Client  *user;
    it++;
    for (; it != request.args.end(); it++)
    {
        user = server->getClientByNick(*it);
        if (channel->isClientConnected(*it) == false)
            Server::sendToClient(client->socketFd, ERR_USERNOTINCHANNEL(client->nickName, *it, channel->name));
        else
            kickFromChannel(client, user, channel);
    }
}

void    kickFromChannel(Client *client, Client *user, Channel *channel)
{
    Server::sendToClient(user->socketFd, RPL_CMD(user->nickName, user->userName, "KICK", ("You have been kicked from " + channel->name + " by " + client->nickName)));
    user->eraseChannel(channel->name);
    channel->eraseClient(user->nickName);
    channel->sendToAllclient(RPL_CMD(client->nickName, client->userName, "KICK", (channel->name + " " + user->nickName)));
}

