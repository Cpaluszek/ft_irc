#include "commands.hpp"
#include "Utils.hpp"

enum topicAction {
	NONE,
	SET_TOPIC,
	PRINT_TOPIC,
	CLEAR_TOPIC
};

topicAction defineTopicAction(const Request &request) {
	if ( request.args.size() == 1)
		return PRINT_TOPIC;
	std::string channelName = request.args[1];
	if ( channelName.find(':') != std::string::npos && channelName.length() == 1)
		return CLEAR_TOPIC;
	if ( channelName.length() > 1 )
		return SET_TOPIC;
	return NONE;
}

static bool	channelExist( Server *server , Client *client, const std::string& channelName ) {
	if (server->isAChannel(channelName ))
		return true;
	Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, channelName));
	return false;
}

bool Request::requestTopicIsValid( Client *client ) const {
	std::string targetChannel = this->args[0];
	if ( targetChannel.find('#', 0) == std::string::npos || targetChannel.length() == 1 ) {
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command));
		return false;
	}
	return true;
}

void printTopic( Client *client, Channel *channel ) {
	if ( channel->getTopic().empty() )
		Server::sendToClient( client->socketFd, RPL_NOTOPIC(client->nickName, channel->getName(), channel->getTopic() ));
	else 	{
		Server::sendToClient( client->socketFd, RPL_TOPIC(client->nickName, channel->getName(), \
													channel->getTopic() ));
		Server::sendToClient( client->socketFd, RPL_TOPICWHOTIME(client->nickName, channel->getName(), \
													channel->getTopicUser(), channel->getTopicTime() ));
	}
}

void clearTopic( Client *client, Channel *channel ) {
	channel->setTopic("" , client->nickName );
	channel->sendToAllClients(RPL_CMD(client->nickName, client->userName, "TOPIC", channel->getName() + " :"));
}

void setTopic(Client *client, Channel *channel, const Request &request ) {
	std::string newTopic = request.args[1];
	newTopic.erase(0, 1);
	channel->setTopic(newTopic, client->nickName );
	channel->sendToAllClients(RPL_CMD(client->nickName, client->userName, "TOPIC", channel->getName() + " :" + newTopic));
}

void topicCmd( Client *client, const Request &request, Server *server ) {
	std::vector<std::string>::const_iterator itArgs = request.args.begin();

	if ( !request.requestTopicIsValid( client ) || !channelExist( server, client, *itArgs ) ) {
		return ;
	}
	// if client is not on channel
	Channel *specificChannel = server->getChannelByName( *itArgs );
	t_channelUser *channelUser = specificChannel->getChannelUserByNick(client->nickName);
	if ( !channelUser ) {
		Server::sendToClient( client->socketFd, ERR_NOTONCHANNEL( client->nickName, specificChannel->getName()));
		return ;
	}

	bool userHasPrivilege = specificChannel->isClientOperator(client->nickName );
	// Exec topic cmd if user has privilege
	switch (defineTopicAction(request)) {
		case PRINT_TOPIC:
			if ( !specificChannel->isClientConnected( client->nickName ) )
				Server::sendToClient( client->socketFd, ERR_NOTONCHANNEL( client->nickName, specificChannel->getName()));
			else
				printTopic( client, specificChannel );
			break;
		case CLEAR_TOPIC:
			if (specificChannel->hasMode('t') && !userHasPrivilege) {
				Server::sendToClient( client->socketFd, ERR_CHANOPRIVSNEEDED( client->nickName, specificChannel->getName()) );
				return ;
			}
			clearTopic( client, specificChannel );
			break;
		case SET_TOPIC:
			if (specificChannel->hasMode('t') && !userHasPrivilege) {
				Server::sendToClient( client->socketFd, ERR_CHANOPRIVSNEEDED( client->nickName, specificChannel->getName()) );
				return ;
			}
			setTopic( client, specificChannel, request );
		default:
			break;
	}
}
