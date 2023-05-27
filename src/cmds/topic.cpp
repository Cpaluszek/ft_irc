//
// Created by aucaland on 5/25/23.
//
#include "commands.hpp"
#include "Utils.hpp"

bool Commands::requestTopicIsValid( Client *client )
{
	if (this->args.empty()) {
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command));
		return false;
	}
	size_t pos;
	std::vector<std::string>::const_iterator itArgs = this->args.begin();
	if ( (pos = Utils::getSemicolPos( *itArgs )) != std::string::npos )
	{
		if ( pos == 0 )
			Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, this->command));
		else
			Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, *itArgs));
	}
	else
	{
		if ( (++itArgs) != this->args.end() && (pos = Utils::getSemicolPos( *itArgs )) != std::string::npos && pos == 0 )
			return true;
		Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, *itArgs));
	}
	return false;
}

void Commands::topicCmd( Client *client )
{
	if ( !requestTopicIsValid( client ) )
		return ;
<<<<<<< HEAD
	std::vector<std::string>::const_iterator itArgs = );

=======
	std::cerr << "ok" << std::endl;
>>>>>>> parent of a2e1325 (commit before creating command class)
}