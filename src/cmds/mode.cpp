/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:44:41 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/12 13:49:03 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "flags.hpp"

static bool formatIsChannel( const std::string& arg ) {
	if ( arg.find('#', 0) != std::string::npos )
		return true;
	return false;
}

static bool	formatIsModestring( const std::string& arg ) {
	if ( ( arg.find('-', 0) == std::string::npos && arg.find('+', 0) == std::string::npos ) )
		return true;
	return false;
}

static bool	channelExist( Server *server , Client *client, const std::string& channel ) {
	if ( server->isAChannel( channel ) )
		return true;
	Server::sendToClient( client->socketFd, ERR_NOSUCHCHANNEL(client->nickName, channel) );
	return false;
}

bool Request::requestModeIsValid( Client *client, Server *server ) const
{
	std::vector<std::string>::const_iterator itArgs = this->args.begin();

	if ( this->args.empty() ) {
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command));
		return false;
	}
	else if ( formatIsChannel( *itArgs ) ) {
		const std::string& channel = *itArgs;
		if ( this->args.size() == 1 )
			return true;
		else if ( !channelExist( server, client, channel ) )
			return false;
	}
	else if ( !server->isUser( *itArgs ) ) {
		Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs ) );
		return false;
	}
	else if ( formatIsModestring( *( itArgs + 1 ) ) )
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command) );
	return true;
}

bool modeParamIsValid(const std::string &param)
{
	int maxClient;
	std::istringstream ss( param );
	ss >> maxClient;
	if ( ss.fail() || !ss.eof() )
		return false;
	return true;
}

bool	containSecondParam( char arg, int typeOfFlag ) {
	if ( arg == 'o' || ( typeOfFlag == ADD && ( arg == 'k' || arg == 'l' ) ) )
		return true;
	return false;
}

bool	setTypeOfFlag( int *typeOfFlag, char c ) {
	switch (c) {
		case '-':
			*typeOfFlag = RM;
			return true;
		case '+':
			*typeOfFlag = ADD;
			return true;
		default:
			return false;
	}
}

static bool lackOfParam( size_t i, const std::string& arg, int numberOfFlagsWithParam, size_t sizeArgs )
{
	return (i == arg.size() - 1 && numberOfFlagsWithParam > (int)sizeArgs - 2);
}

static bool checkAndFillSecondParam( Client *client, std::map<int, std::string> *flagsMap, int *numberOfFlagsWithParam, \
	const Request &request, std::string *secondParam, const std::string& arg, size_t i, size_t sizeArgs )
{
	size_t requestSize = request.args.size();
	size_t theoricalSecondParamPos = 2 + *numberOfFlagsWithParam;

	if ( theoricalSecondParamPos + 1 <= requestSize )
	{
		*secondParam = request.args[ theoricalSecondParamPos ];
		++(*numberOfFlagsWithParam);
	}
	else if ( lackOfParam( i, arg, ++(*numberOfFlagsWithParam), sizeArgs ) ) 	{
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, request.command) );
		flagsMap->clear();
		return false;
	}
	return true;
}

/***
 *
 * @param client
 * @param request
 * @param mode : USERMOD or CHANNELMOD
 * @return a map<int, std::string>, string are empty when flags doesn't need secondParam. Int contains all flags or UNKNOWN_FLAG if not known. Return Empty map if error
 */
std::map<int, std::string> getFlags( Client *client, const Request &request, int mode ) {
	std::map<int, std::string>					flagsMap;
	std::vector<std::string>::const_iterator	itArgs = (request.args.begin() + 1);
	const std::string& 							arg = *itArgs;
	std::string									secondParam;
	size_t										sizeArgs = request.args.size();
	int											numberOfFlagsWithParam = 0;
	int											typeOfFlag = ADD;


	for (size_t i = 0; i < itArgs->length(); ++i) {
		if ( setTypeOfFlag( &typeOfFlag, arg[i] ) )
			continue;
		if ( containSecondParam( arg[i], typeOfFlag ) )
			if ( !checkAndFillSecondParam( client, &flagsMap, &numberOfFlagsWithParam, request, &secondParam, arg, i, sizeArgs ) )
				return flagsMap;

		switch ( arg[i] ) {
			case 'i' :
				typeOfFlag == ADD ? flagsMap[ I_ADD_INVITEONLY_CHANNELMOD ] = "" : flagsMap[ I_RM_INVITEONLY_CHANNELMOD ] = "";
				break;
			case 'o': {
				if ( mode == USERMOD ) {
					if ( typeOfFlag == ADD )
						flagsMap[ O_ADD_OP_USERMOD ] = secondParam;
					else
						flagsMap[ O_RM_OP_USERMOD ] = secondParam;
				}
				else {
					if ( typeOfFlag == ADD )
						flagsMap[ O_ADD_OP_CHANNELMOD ] = secondParam;
					else
						flagsMap[ O_RM_OP_CHANNELMOD ] = secondParam;
				}
				break;
			}
			case 't': {
				typeOfFlag == ADD ? flagsMap[ T_ADD_PROTECTEDTOPIC_CHANNELMOD ] = "" : flagsMap[ T_RM_PROTECTEDTOPIC_CHANNELMOD ] = "";
				break;
			}
			case 'k': {
				if ( typeOfFlag == ADD )
					flagsMap[K_ADD_KEY_CHANNELMOD] = secondParam;
				else
					flagsMap[ K_RM_KEY_CHANNELMOD ] = "";
				break;
			}
			case 'l': {
				if ( typeOfFlag == ADD )
				{
					if ( !modeParamIsValid(secondParam))
					{
						flagsMap.clear();
						Server::sendToClient( client->socketFd, ERR_MSG(std::string("Client MaxLimit must be a positive number")));
						return flagsMap;
					}
					flagsMap[ L_ADD_CLIENTLIMIT_CHANNELMOD ] = secondParam;
				}
				else
					flagsMap[ L_RM_CLIENTLIMIT_CHANNELMOD ] = "";
				break;
			}
			case 'n': {
				typeOfFlag == ADD ? flagsMap[ N_ADD_CHANNELMODE ] = "" : flagsMap[ N_RM_CHANNELMODE ] = "";
				break;
			}
			case ' ':
				return flagsMap;
			default: {
				if ( mode == USERMOD )
					flagsMap[ UNKNOWN_FLAG ] += arg[i];
				break;
			}
		}
	}
	return flagsMap;
}


static void executeModeCmd(Client *client, Server *server, const std::map<int, std::string> &flagsMap, Channel *channel)
{
	std::map<int, std::string>::const_iterator itFlags = flagsMap.begin();
	std::map<int, std::string>::const_iterator itFlagsEnd = flagsMap.end();

	for (; itFlags != itFlagsEnd ; ++itFlags) {
		std::string flagParam = itFlags->second;
		switch ( itFlags->first ) {
			case O_ADD_OP_CHANNELMOD:
			{
				channelUser *user = channel->getChannelUserByNick(flagParam);
				if (user == NULL) {
					Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, flagParam));
					break ;
				}
				user->userMode = "o";
				user->prefix = "@";
				channel->sendToAllClients(RPL_UPDATE_USER_CHAN_MODE(client->nickName, client->userName, channel->getName(), "+", "o" , user->client->nickName));
				break;
			}
			case O_RM_OP_CHANNELMOD:
			{
				channelUser *user = channel->getChannelUserByNick(flagParam);
				if (user == NULL) {
					Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, flagParam));
					break ;
				}
				user->userMode = "";
				user->prefix = "";
				channel->sendToAllClients(RPL_UPDATE_USER_CHAN_MODE(client->nickName, client->userName, channel->getName(), "-", "o" , user->client->nickName));
				break;
			}
			case O_ADD_OP_USERMOD:
			{
				Client *user = server->getClientByNick(flagParam);
				if (user == NULL) {
					Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, flagParam));
					break ;
				}
				user->addMode('o');
				break;
			}
			case O_RM_OP_USERMOD:
			{
				Client *user = server->getClientByNick(flagParam);
				if (user == NULL) {
					Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, flagParam));
					break ;
				}
                user->removeMode('o');
				break;
			}
			case L_ADD_CLIENTLIMIT_CHANNELMOD:
			{
				std::istringstream cpp98Sucks(flagParam);
				int limitValue;
				cpp98Sucks >> limitValue;
				if (cpp98Sucks.fail() || !cpp98Sucks.eof() || limitValue <= 0 || limitValue > 4096) {
					Server::sendToClient( client->socketFd, ERR_MSG(std::string("Client MaxLimit must be a positive number")));
					break ;
				}
				channel->setClientLimit(limitValue);
				channel->addMode('l');
				break;
			}
			case L_RM_CLIENTLIMIT_CHANNELMOD:
				channel->removeMode('l');
				break;
			case I_ADD_INVITEONLY_CHANNELMOD:
				channel->addMode('i');
				break;
			case I_RM_INVITEONLY_CHANNELMOD:
				channel->removeMode('i');
				break;
			case T_ADD_PROTECTEDTOPIC_CHANNELMOD:
				channel->addMode('t');
                channel->updateTopic(flagParam, client->nickName);
				break;
			case T_RM_PROTECTEDTOPIC_CHANNELMOD:
				channel->removeMode('t');
				break;
			case K_ADD_KEY_CHANNELMOD: {
				if (flagParam.find(' ') != std::string::npos) {
					Server::sendToClient( client->socketFd, ERR_INVALIDKEY(client->nickName, channel->getName()));
					break ;
				}
                channel->addMode('k');
				channel->setKey(flagParam);
                break;
			}
            case K_RM_KEY_CHANNELMOD:
                channel->removeMode('k');
				break;
			case N_ADD_CHANNELMODE:
				channel->addMode('n');
				break;
			case N_RM_CHANNELMODE:
				channel->removeMode('n');
				break;
			case UNKNOWN_FLAG:
				Server::sendToClient( client->socketFd, ERR_UMODEUNKNOWNFLAG( client->nickName, flagParam ) );
				break;
			default:
				break;
		}
	}
}

void modesOverview( Channel *channel, Client *client ) {
	Server::sendToClient( client->socketFd, RPL_CHANNELMODEIS(client->nickName, channel->getName(), channel->getMode() ));
	Server::sendToClient( client->socketFd, RPL_CREATIONTIME(client->nickName, channel->getName(), channel->getCreationTime() ));
}

void getFlagsAndPrintChannelMode( Client *client, const Request &request, Server *server , Channel **channel, std::map<int, std::string> *flagsMap )
{
	*channel = server->getChannelByName( *request.args.begin() );
	if ( request.args.size() == 1 ) {
		modesOverview( *channel, client );
		return ;
	}
	if ( *channel && !(*channel)->isClientOperator( client->nickName ) ) {
		Server::sendToClient( client->socketFd, ERR_CHANOPRIVSNEEDED( client->nickName, (*channel)->getName()) );
		return ;
	}
	*flagsMap = getFlags( client, request, CHANNELMOD );
}

void mode( Client *client, const Request &request, Server *server )
{
	Channel 									*channel = NULL;
	std::map<int, std::string>					flagsMap;
	std::vector<std::string>::const_iterator	itRequest = request.args.begin();

	if ( !request.requestModeIsValid( client, server ) )
		return ;
	// usermod or channel mod ==> get flags
	if ( itRequest->find('#', 0) != std::string::npos )
		getFlagsAndPrintChannelMode( client, request, server, &channel, &flagsMap );
	else
		flagsMap = getFlags( client, request, USERMOD );
	//Exec
	if ( !flagsMap.empty() )
		executeModeCmd(client, server, flagsMap, channel);
//	std::map<int, std::string>::iterator itprint = flagsMap.begin(); //print map DEBUG
//	for (;itprint != flagsMap.end() ; itprint++) {
//		std::cout << itprint->first << std::endl;
//	}
}
