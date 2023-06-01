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
    std::cout << "OK" <<std::endl;
    std::vector<std::string>::const_iterator it = request.args.begin();
//    it++;
    std::map<std::string, Channel*>::const_iterator channelIt = server->getChannelByName(*it);
    if (channelIt == server->getChannelEnd())
    {
        Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, *it));
        std::cout << "OK2" <<std::endl;
        return ;
    }
    else if (client->isOnChannel(*it) == false)
    {
        Server::sendToClient(client->socketFd, ERR_NOTONCHANNEL(client->nickName, *it));
        std::cout << "OK3" <<std::endl;
        return ;
    }
    std::cout << "OK4" << *it << std::endl;
    std::string channelName = *it;
    Client  *user;
    it++;
    std::cout << "OK5" << *it<< std::endl;
    for (; it != request.args.end(); it++)
    {
        user = server->getClientByNick(*it);
        std::cout << "OK6" << user->nickName <<std::endl;
        if (user->isOnChannel(channelName) == false)
            Server::sendToClient(client->socketFd, ERR_USERNOTINCHANNEL(client->nickName, *it, channelName));
        else
            kickFromChannel(client, user, channelIt->second);
        std::cout << "OK7" <<std::endl;
    }
}

void    kickFromChannel(Client *client, Client *user, Channel *channel)
{
    std::cout << "OK77" <<std::endl;
    user->eraseChannel(channel->name);
    std::cout << "OK8" <<std::endl;
    channel->eraseClient(user->nickName);
    std::cout << "OK9" <<std::endl;
    channel->sendToAllclient(RPL_CMD(client->nickName, client->userName, "KICK", (channel->name + " has kicked " + user->nickName)));
}

