//
// Created by aucaland on 5/25/23.
//
#include "commands.hpp"
#include "Utils.hpp"

bool Commands::requestTopicIsValid( Client *client )
{
	std::vector<std::string>::const_iterator itArgs = this->args.begin();
	if ( (itArgs)->find('#', 0) == std::string::npos || itArgs->length() == 1 ) {
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command));
		return false;
	}
	return true;
}

void Commands::topicCmd( Client *client )
{
	if ( !requestTopicIsValid( client ) )
		return ;
	std::vector<std::string>::const_iterator itArgs = );

}