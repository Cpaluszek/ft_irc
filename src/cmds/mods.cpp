//
// Created by aucaland on 5/30/23.
//
#include "Server.hpp"
#include "flags.hpp"

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

bool	modeParamIsValid( int flag, std::string param ) //TODO: change if only one flag
{
		if ( flag ==  L_ADD_CLIENTLIMIT_CHANNELMOD )
		{
			int maxClient;
			std::istringstream ss( param );
			ss >> maxClient;
			if ( ss.fail() && !ss.eof() )
				return false;
		}
		return true;
}

bool	containSecondParam( char arg )
{
	if ( arg == 'o' || arg == 'k' || arg == 'l' )
		return true;
	return false;
}

bool	numberParamIsCorrect( Client *client, const Request &request, char arg, size_t sizeArgs, size_t *numberOfFlagsWithParam )
{
	//Check number param is correct
	if ( containSecondParam( arg ) )
		(*numberOfFlagsWithParam)++;
	if ( *numberOfFlagsWithParam > sizeArgs - 2 )
	{
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, request.command));
		return false;
	}
	return true;
}

std::map<int, std::string> getFlags( Client *client, const Request &request, int mode )
{
	std::map<int, std::string>					flagsMap;
	std::vector<std::string>::const_iterator	itArgs = (request.args.begin() + 1);
	const std::string& arg = *itArgs;
	size_t	sizeArgs = request.args.size();
	size_t numberOfFlagsWithParam = 0;
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
		if ( typeOfFlag == ADD && !numberParamIsCorrect( client, request, arg[i], sizeArgs, &numberOfFlagsWithParam ))
		{
			flagsMap.clear();
			return flagsMap;
		}
		std::string secondParam = "";
		if ( ( itArgs + (int)numberOfFlagsWithParam + 1 ) != request.args.end() && i < arg.size() - 1 )
			secondParam = *(itArgs + (int)numberOfFlagsWithParam + 1);
		else if ( i == arg.size() - 1 && numberOfFlagsWithParam < sizeArgs - 2 )
		{
			Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, request.command));
			flagsMap.clear();
			return flagsMap;
		}
		std::cerr << "i = " << i << ", countparam=" << numberOfFlagsWithParam << ", secondparam=" << secondParam << ", sizeArgs=" << sizeArgs << ", sizeArg=" << arg.size() << std::endl;

		// Get mod in a Vector<int>, to call them more explicitly
		switch ( arg[i] ) {
			case 'i' :
				typeOfFlag == ADD ? flagsMap[ I_ADD_INVITEONLY_CHANNELMOD ] = "" : flagsMap[ I_RM_INVITEONLY_CHANNELMOD ] = "";
				break;
			case 'o':
			{
				if ( mode == USERMOD )
				{
					if ( typeOfFlag == ADD )
					{
						flagsMap[ O_ADD_OP_USERMOD ] = secondParam;
					}
					else
						flagsMap[ O_RM_OP_USERMOD ] = "";
				}
				else
				{
					if ( typeOfFlag == ADD )
					{
						flagsMap[ O_ADD_OP_CHANNELMOD ] = secondParam;
					}
					else
						flagsMap[ O_RM_OP_CHANNELMOD ] = "";
				}
				break;
			}
			case 't':
			{
				typeOfFlag == ADD ? flagsMap[ T_ADD_PROTECTEDTOPIC_CHANNELMOD ] = "" : flagsMap[ T_RM_PROTECTEDTOPIC_CHANNELMOD ] = "";
				break;
			}
			case 'k':
			{
				if ( typeOfFlag == ADD ) {
					flagsMap[K_ADD_KEY_CHANNELMOD] = secondParam;
				}
				else
					flagsMap[ K_RM_KEY_CHANNELMOD ] = "";
				break;
			}
			case 'l':
			{
				if ( typeOfFlag == ADD )
				{
					if ( !modeParamIsValid( L_ADD_CLIENTLIMIT_CHANNELMOD, secondParam ))
					{
						flagsMap.clear();
						Server::sendToClient( client->socketFd, "MaxLimit Client has to be a number\r\n");
						return flagsMap;
					}
					flagsMap[ L_ADD_CLIENTLIMIT_CHANNELMOD ] = secondParam;
				}
				else
					flagsMap[ L_RM_CLIENTLIMIT_CHANNELMOD ] = "";
				break;
			}
			case ' ':
				return flagsMap;
			default:
				break;
		}
	}
	return flagsMap;
}


static void executeModeCmd( Client *client, Server *server, const Request &request, const std::map<int, std::string> &flagsMap, Channel *channel )
{
	(void)client;
	(void)server;
	(void)request;
	(void)flagsMap;
	(void)channel;
	std::map<int, std::string>::const_iterator itFlags = flagsMap.begin();
	std::map<int, std::string>::const_iterator itFlagsEnd = flagsMap.end();

	for (; itFlags != itFlagsEnd ; ++itFlags) {
		std::string flagParam = itFlags->second;
		switch ( itFlags->first ) {
			case O_ADD_OP_CHANNELMOD:
				break;
			case O_RM_OP_CHANNELMOD:
				break;
			case O_ADD_OP_USERMOD:
				break;
			case O_RM_OP_USERMOD:
				break;
			case L_ADD_CLIENTLIMIT_CHANNELMOD:
				break;
			case L_RM_CLIENTLIMIT_CHANNELMOD :
				break;
			case I_ADD_INVITEONLY_CHANNELMOD:
				break;
			case I_RM_INVITEONLY_CHANNELMOD:
				break;
			case T_ADD_PROTECTEDTOPIC_CHANNELMOD:
				break;
			case T_RM_PROTECTEDTOPIC_CHANNELMOD:
				break;
			case K_ADD_KEY_CHANNELMOD:
				break;
			case K_RM_KEY_CHANNELMOD:
				break;
			default:
				break;
		}
	}
}

void mode( Client *client, const Request &request, Server *server )
{
	(void) client;
	(void) server;

	Channel *channel;
	if ( !request.requestModeIsValid( client, server ) )
		return ;
	std::map<int, std::string>	flagsMap;
	if ( request.args.begin()->find('#', 0) != std::string::npos )
	{
		channel = server->getChannelByName( *request.args.begin() )->second;
		flagsMap = getFlags( client, request, CHANNELMOD );
	}
	else
	{
		channel = NULL;
		flagsMap = getFlags( client, request, USERMOD );
	}
	if ( flagsMap.empty() )
		return ;
	if ( channel && channel->getChannelUserByNick( client->nickName )->userMode.find('o') == std::string::npos )//TODO: implement general function to find channel mod
	{
		Server::sendToClient( client->socketFd, ERR_CHANOPRIVSNEEDED( client->nickName, channel->name));
		return ;
	}
	executeModeCmd( client, server, request, flagsMap, channel );
	std::map<int, std::string>::iterator itprint = flagsMap.begin(); //print map DEBUG
	for (;itprint != flagsMap.end() ; itprint++) {
		std::cout << itprint->first << std::endl;
	}
}
