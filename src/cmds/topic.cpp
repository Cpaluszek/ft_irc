//
// Created by aucaland on 5/25/23.
//
#include "commands.hpp"

size_t 	containSemicol( std::string arg )
{
	size_t pos = arg.find( ':' );
	return pos;
}

bool requestIsValid( Client *client, const Request &request )
{
	if (request.args.empty()) {
		Server::sendToClient( client->socketFd, ERR_NORECIPIENT( client->nickName, request.command));
		return false;
	}
	size_t pos;
	std::vector<std::string>::const_iterator itArgs = request.args.begin();
	if ( (*itArgs)[0] != '#' )
		Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, *itArgs));
	if ( (pos = containSemicol( *itArgs )) != std::string::npos )
	{
		if ( pos == 0 )
			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
		else
			Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, *itArgs));
	}
	else
	{
		if ( pos == 0 )
			return true;
		Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs));
	}
	return false;
}

void topicCmd( Client *client, const Request &request, Server *server )
{
	if ( !requestIsValid( client, request ) )
		return ;
	std::cerr << "ok" << std::endl;
}