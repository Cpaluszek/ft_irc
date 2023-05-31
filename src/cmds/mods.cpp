//
// Created by aucaland on 5/30/23.
//
#include "Server.hpp"
#include "flags.hpp"

#define CHANNELMOD	35
#define USERMOD		36
#define ADD			37
#define RM			38

static bool	channelExist( Server *server , Client *client, const std::string& channel )
{
	if (server->isAChannel( channel ))
		return true;
	Server::sendToClient(client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, channel));
	return false;
}

bool Request::requestModeIsValid( Client *client, Server *server ) const
{
	std::vector<std::string>::const_iterator itArgs = this->args.begin();

	if ( this->args.empty() || (itArgs + 1) == this->args.end() )
	{
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command));
		return false;
	}
	else if ( itArgs->find('#', 0) != std::string::npos )
	{
		std::string channel = *itArgs;
		if ( itArgs->length() == 1 )
			Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command));
		else if ( !channelExist( server, client, channel ) )
			return false;
	}
	else if ( !server->isUser( *itArgs ))
	{
		Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs));
		return false;
	}
	else if ( ( (itArgs + 1)->find('-', 0) == std::string::npos && (itArgs + 1)->find('+', 0) == std::string::npos ) )
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command));
	return true;
}

std::vector<int> getFlags( const Request &request, int mode )
{
	std::vector<int>					flags;
	std::vector<std::string>::const_iterator	itArgs = (request.args.begin() + 1);
	const std::string& arg = *itArgs;
	int typeOfFlag = ADD;

	for (size_t i = 0; i < itArgs->length(); ++i) {
		if ( arg[i] == '-')
		{
			typeOfFlag = RM;
			continue;
		}
		else if ( arg[i] == '+' )
		{
			typeOfFlag = ADD;
			continue;
		}
		// Get mod in a Vector<int>, to call them more explicitly
		switch ( arg[i] ) {
			case 'i' :
			{
				if ( mode == USERMOD )
				{
					if ( typeOfFlag == ADD )
						flags.push_back( I_ADD_INVISIBLE_USERMOD );
					else
						flags.push_back( I_RM_INVISIBLE_USERMOD );
				}
				else
				{
					if ( typeOfFlag == ADD )
						flags.push_back( I_ADD_INVITEONLY_CHANNELMOD );
					else
						flags.push_back( I_RM_INVITEONLY_CHANNELMOD );
				}
				break;
			}
			case 'o':
			{
				if ( mode == USERMOD )
				{
					if ( typeOfFlag == ADD )
						flags.push_back( O_ADD_OP_USERMOD );
					else
						flags.push_back( O_RM_OP_USERMOD );
				}
				break;
			}
			case 'r':
			{
				if ( mode == USERMOD )
				{
					if ( typeOfFlag == ADD )
						flags.push_back( R_ADD_REGISITERED_USERMOD );
					else
						flags.push_back( R_RM_REGISTERED_USERMOD );
				}
				break;
			}
			case 'e':
			{
				if ( typeOfFlag == ADD )
					flags.push_back( E_ADD_BANEXCEPTIONLIST_CHANNELMOD );
				else
					flags.push_back( E_ADD_BANEXCEPTIONLIST_CHANNELMOD );
				break;
			}
			case 'b':
				if ( typeOfFlag == ADD )
					flags.push_back( B_ADD_BANLIST_CHANNELMOD );
				else
					flags.push_back( B_ADD_BANLIST_CHANNELMOD );
				break;
			case ' ':
				return flags;
			default:
				break;
		}
	}
	return flags;
}//TODO: implement all mod

void mode( Client *client, const Request &request, Server *server )
{
	(void) client;
	(void) server;

	if ( !request.requestModeIsValid( client, server ) )
		return ;
	std::vector<int>	flags;
	std::vector<std::string>::const_iterator itArgs = request.args.begin();
	if ( itArgs[0].find('#', 0) != std::string::npos )
		flags = getFlags( request, CHANNELMOD );
	else
		flags = getFlags( request, USERMOD );
	std::vector<int>::iterator itprint = flags.begin();
	for (;itprint != flags.end() ; itprint++) {
		std::cout << *itprint << std::endl;
	}
}
