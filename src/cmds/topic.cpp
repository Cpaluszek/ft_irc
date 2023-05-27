//
// Created by aucaland on 5/25/23.
//
#include "commands.hpp"
#include "Utils.hpp"

bool Request::requestTopicIsValid( Client *client ) const
{
	std::vector<std::string>::const_iterator itArgs = this->args.begin();
	if ( (itArgs)->find('#', 0) == std::string::npos || itArgs->length() == 1 ) {
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command));
		return false;
	}
	return true;
}

void topicCmd( Client *client, const Request &request, Server *server )
{
	(void) server;
	if ( !request.requestTopicIsValid( client ) )
		return ;
	std::vector<std::string>::const_iterator itArgs = request.args.begin();

}