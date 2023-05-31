//
// Created by aucaland on 5/25/23.
//
#include "commands.hpp"
#include "Utils.hpp"

# define NONE			0
# define SET_TOPIC		1
# define PRINT_TOPIC	2
# define CLEAR_TOPIC	3

int	defineTopicAction( std::vector<std::string>::const_iterator itArgs )
{
	if ( (itArgs + 1)->empty() )
		return PRINT_TOPIC;
	else if ( (itArgs + 1)->find(':') && (itArgs + 1)->length() == 1)
		return CLEAR_TOPIC;
	else if ( (itArgs + 1)->length() > 1 )
		return SET_TOPIC;
	return NONE;
}

static bool	channelExist( Server *server , Client *client, const std::string& channel )
{
	if (server->isAChannel( channel ))
		return true;
	Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, channel));
	return false;
}

bool Request::requestTopicIsValid( Client *client ) const
{
	std::vector<std::string>::const_iterator itArgs = this->args.begin();
	if ( (itArgs)->find('#', 0) == std::string::npos || itArgs->length() == 1 ) {
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command));
		return false;
	}
	return true;
}

void printTopic( Client *client, Channel *specificChannel )
{
	if ( specificChannel->getTopic().empty() )
		Server::sendToClient( client->socketFd, RPL_NOTOPIC( client->nickName, specificChannel->name, specificChannel->getTopic() ));
	else
	{
		Server::sendToClient( client->socketFd, RPL_TOPIC( client->nickName, specificChannel->name, \
													specificChannel->getTopic() ));
		Server::sendToClient( client->socketFd, RPL_TOPICWHOTIME( client->nickName, specificChannel->name, \
													specificChannel->getTopicUser(), specificChannel->getTopicTime() ));
	}
}

void clearTopic( Client *client, Channel *specificChannel )
{
	specificChannel->setTopic( "" , client->nickName );
	specificChannel->sendToAllclient(TOPIC( specificChannel->name, "" ) );
}

void setTopic( Client *client, Channel *specificChannel, const Request &request )
{
	std::vector<std::string>::const_iterator itArgs = ++request.args.begin();
	std::string newTopic = itArgs->substr( 1, itArgs->length() );
	specificChannel->setTopic( newTopic, client->nickName );
	specificChannel->sendToAllclient(TOPIC( specificChannel->name, newTopic ) );
}

void topicCmd( Client *client, const Request &request, Server *server )
{
	std::vector<std::string>::const_iterator itArgs = request.args.begin();
	if ( !request.requestTopicIsValid( client ) || !channelExist( server, client, *itArgs ) ) {
		return ;
	}


	Channel *specificChannel = server->getChannelByName( *itArgs )->second;
	switch ( defineTopicAction( itArgs ) ) {
		case PRINT_TOPIC:
			if ( !specificChannel->isClientConnected( client->nickName ) )
				Server::sendToClient( client->socketFd, ERR_NOTONCHANNEL( client->nickName, specificChannel->name ));
			else
				printTopic( client, specificChannel );
			break;
		case CLEAR_TOPIC:
			clearTopic( client, specificChannel );//TODO: Check Permissions;
			break;
		case SET_TOPIC:
			setTopic( client, specificChannel, request );//TODO: Check Permissions
		default:
			break;
	}

}