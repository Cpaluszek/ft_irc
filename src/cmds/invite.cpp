#include "commands.hpp"

// Todo: check each error
// Todo: do we allow invite with no parameters to get the invite list ?

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#invite-message)
void inviteCmd(Client *client, const Request &request, Server *server) {
	if (request.args.empty() || request.args.size() < 2) {
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, request.command));
		return ;
	}

	std::string targetNick = request.args[0];
	std::string targetChannel = request.args[1];

	// Todo: check if we need to remove '#' on channel name

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

	// Check if the user is already connected on the channel - ERR_USERONCHANNEL
	if (channel->isClientConnected(targetNick)) {
		Server::sendToClient(client->socketFd, ERR_USERONCHANNEL(client->nickName, targetNick, targetChannel));
		return ;
	}

	// RPL_INVITING numeric to the command issuer
	Server::sendToClient(client->socketFd, RPL_INVITING(client->nickName, targetNick, targetChannel));
	// INVITE message the the issuer as <source> to the target user
	channel->addInvite(targetNick);
	Client *targetClient = server->getClientByNick(targetNick);
	Server::sendToClient(targetClient->socketFd, RPL_INVITE(client->nickName, client->userName, targetChannel, targetNick));
}