
#include "commands.hpp"

void	quitChannel(Client *client, Channel *channel);

void	partCmd(Client *client, const Request &request, Server *server)
{
	if (request.args.empty()){
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, request.command));
		return ;
	}
	std::vector<std::string>::const_iterator it = request.args.begin();
	for (; it != request.args.end(); it++)
	{
		std::map<std::string, Channel*>::const_iterator channelIt = server->getChannelByName(*it);
		if (channelIt == server->getChannelEnd())
			Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, *it));
		else if (!client->isOnChannel(*it))
			Server::sendToClient(client->socketFd, ERR_NOTONCHANNEL(client->nickName, *it));
		else
		{
			quitChannel(client, channelIt->second);
			break ;
		}
	}

}

// Todo: leave all channels is not always correct reason
void	quitChannel(Client *client, Channel *channel)
{
	client->eraseChannel(channel->name);
	size_t channelUserCount = channel->getClientCount();
	channel->eraseClient(client->nickName);
	if (channelUserCount > 1) {
		channel->sendToAllclient(RPL_CMD(client->nickName, client->userName, "PART", (channel->name + " has leaveAllChannels")));
	}
}
