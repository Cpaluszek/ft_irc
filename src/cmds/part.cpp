
#include "commands.hpp"

void leaveChannel(Client *client, Channel *channel, const std::string& reason);

// Todo: check for multi channel part command
void	partCmd(Client *client, const Request &request, Server *server) {
	if (request.args.empty()){
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, request.command));
		return ;
	}
	std::string targetChannel = request.args[0];
	std::string reason = request.args.size() > 1 ? request.args[1] : "No reason";
	Channel *channel = server->getChannelByName(targetChannel);
	if (channel == NULL)
		Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, targetChannel));
	else if (!client->isOnChannel(targetChannel))
		Server::sendToClient(client->socketFd, ERR_NOTONCHANNEL(client->nickName, targetChannel));
	else
		leaveChannel(client, channel, reason);
}

void leaveChannel(Client *client, Channel *channel, const std::string& reason) {
	client->eraseChannel(channel->getName());
	size_t channelUserCount = channel->getClientCount();
	channel->eraseClient(client->nickName);
	if (channelUserCount > 1) {
		channel->sendToAllClients(
				RPL_CMD(client->nickName, client->userName, "PART", (channel->getName() + " " + reason)));
	}
}
