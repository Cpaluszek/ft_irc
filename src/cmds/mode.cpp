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
		std::string channel = *itArgs;
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
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, this->command ) );
	return true;
}

bool	modeParamIsValid( int flag, const std::string& param ) //TODO: change if only one flag
{
	(void) flag;
	int maxClient;
	std::istringstream ss( param );
	ss >> maxClient;
	if ( ss.fail() && !ss.eof() )
		return false;
	return true;
}

bool	containSecondParam( char arg ) {
	if ( arg == 'o' || arg == 'k' || arg == 'l' )
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

static bool checkAndFillSecondParam( Client *client, std::map<int, std::string> flagsMap, int &numberOfFlagsWithParam, \
	std::vector<std::string>::const_iterator itArgs, const Request &request, \
		std::string secondParam, const std::string arg, size_t i, size_t sizeArgs )
{
	std::vector<std::string>::const_iterator itSecondParam = itArgs + numberOfFlagsWithParam;

	numberOfFlagsWithParam++;
	if ( itSecondParam != request.args.end() && i < arg.size() - 1 )
		secondParam = *( itSecondParam );
	else if ( lackOfParam( i, arg, numberOfFlagsWithParam, sizeArgs ) ) 	{
		Server::sendToClient( client->socketFd, ERR_NEEDMOREPARAMS( client->nickName, request.command) );
		flagsMap.clear();
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
		if ( containSecondParam( arg[i]) )
			if ( !checkAndFillSecondParam( client, flagsMap, numberOfFlagsWithParam, itArgs, request, secondParam, arg, i, sizeArgs ) )
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
						flagsMap[ O_RM_OP_USERMOD ] = "";
				}
				else {
					if ( typeOfFlag == ADD )
						flagsMap[ O_ADD_OP_CHANNELMOD ] = secondParam;
					else
						flagsMap[ O_RM_OP_CHANNELMOD ] = "";
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
					if ( !modeParamIsValid( L_ADD_CLIENTLIMIT_CHANNELMOD, secondParam ) )
					{
						flagsMap.clear();
						Server::sendToClient( client->socketFd, "Client MaxLimit has to be a number\r\n");
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
			default: {
				if ( mode == USERMOD )
					flagsMap[ UNKNOWN_FLAG ] += arg[i];
				break;
			}
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

    //todo : checker si le client existe
	for (; itFlags != itFlagsEnd ; ++itFlags) {
		std::string flagParam = itFlags->second;
		switch ( itFlags->first ) {
			case O_ADD_OP_CHANNELMOD:
			{
                channel->getClients().find(flagParam)->second.userMode = "o";
				break;
			}
			case O_RM_OP_CHANNELMOD:
			{
                channel->getClients().find(flagParam)->second.userMode = "";
				break;
			}
			case O_ADD_OP_USERMOD:
			{
                server->getClientByNick(flagParam)->addMode('o');
				break;
			}
			case O_RM_OP_USERMOD:
			{
                server->getClientByNick(flagParam)->removeMode('o');
				break;
			}
			case L_ADD_CLIENTLIMIT_CHANNELMOD:
            {
                channel->addMode('l');
				channel->setClientLimit(flagParam);
				// check that atoi(std::string) <= 4096 && >0
                // si cest bon appel a la fncton de limit channel
                break;
            }
			case L_RM_CLIENTLIMIT_CHANNELMOD:
			{

				channel->removeMode('l');
				break;
			}
			case I_ADD_INVITEONLY_CHANNELMOD:
			{
				channel->addMode('i');
				break;
			}
			case I_RM_INVITEONLY_CHANNELMOD:
			{
				channel->removeMode('i');
				break;
			}
			case T_ADD_PROTECTEDTOPIC_CHANNELMOD:
			{
				channel->addMode('t');
                channel->updateTopic(flagParam, client->nickName);
				break;
			}
			case T_RM_PROTECTEDTOPIC_CHANNELMOD:
			{
				channel->removeMode('t');
				break;
			}
			case K_ADD_KEY_CHANNELMOD:
			{
                channel->addMode('k');
				channel->setKey(flagParam);
                break;
			}
            case K_RM_KEY_CHANNELMOD:
			{
                channel->removeMode('k');
				break;
			}
			case UNKNOWN_FLAG:
			{
				Server::sendToClient( client->socketFd, ERR_UMODEUNKNOWNFLAG( client->nickName, flagParam ) );
				break;
			}
			default:
				break;
		}
	}
}

void modesOverview( Channel *channel, Client *client ) {
	Server::sendToClient( client->socketFd, RPL_CHANNELMODEIS(client->nickName, channel->getName(), channel->getMode() ));
	Server::sendToClient( client->socketFd, RPL_CREATIONTIME(client->nickName, channel->getName(), channel->getCreationTime() ));
}

void getFlagsAndPrintChannelMode( Client *client, const Request &request, Server *server , Channel *channel, std::map<int, std::string> *flagsMap )
{
	channel = server->getChannelByName( *request.args.begin() );
	if ( request.args.size() == 1 ) {
		modesOverview( channel, client );
		return ;
	}
	if ( channel && !channel->isClientOperator( client->nickName ) ) {
		Server::sendToClient( client->socketFd, ERR_CHANOPRIVSNEEDED( client->nickName, channel->getName()) );
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
		getFlagsAndPrintChannelMode( client, request, server, channel, &flagsMap );
	else
		flagsMap = getFlags( client, request, USERMOD );
	//Exec
	if ( !flagsMap.empty() )
		executeModeCmd( client, server, request, flagsMap, channel );
//	std::map<int, std::string>::iterator itprint = flagsMap.begin(); //print map DEBUG
//	for (;itprint != flagsMap.end() ; itprint++) {
//		std::cout << itprint->first << std::endl;
//	}
}
