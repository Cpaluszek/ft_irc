#include "commands.hpp"

bool	targetIsChannel( std::string target )
{
	if ( target[0] == '#')
		return true;
	return false;
}

bool	channelExist( Server *server , Client *client, const std::string& channel )
{
	if (server->isAChannel( channel ))
		return true;
	Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, channel));
	return false;
}

std::string extractTarget( const Request &request )
{
	return (*request.args.begin());
}

std::string extractMessage( const Request &request )
{
	std::string message = *(++request.args.begin());
	return (message.substr(1, message.length()));
}

//send to all users in channel except source client, TODO : class method
void	sendMessageToAllChannelUsers( std::string message, std::string channel, Client *client)
{
	std::map<std::string, Channel*> channelsMap= client->getChannels();
	std::map<std::string, Channel*>::iterator it = channelsMap.find( channel );
	if (it != channelsMap.end())
	{
		std::map<std::string, t_channelUser> mapChannelUsers = it->second->getClients();
		std::map<std::string, t_channelUser>::iterator itMapChannelUsers = mapChannelUsers.begin();
		for (; itMapChannelUsers != mapChannelUsers.end() ; ++itMapChannelUsers) {
			{
				if ( itMapChannelUsers->second.client->nickName != client->nickName)
					Server::sendToClient(itMapChannelUsers->second.client->socketFd, message);
			}
		}
	}
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#privmsg-message)
size_t 	containSemicol( std::string arg )
{
	size_t pos = arg.find( ':' );
	return pos;
}

bool	requestIsValid( Client *client, const Request &request )
{
	if (request.args.empty()) {
		Server::sendToClient( client->socketFd, ERR_NORECIPIENT( client->nickName, request.command));
		return false;
	}
	size_t pos;
	std::vector<std::string>::const_iterator itArgs = request.args.begin();
	if ( (pos = containSemicol( *itArgs )) != std::string::npos ) // If ":" is in first arg
	{
		if ( pos == 0 )
			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
		else
			Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs));
		return false;
	}
	else //If ":" is not in first arg
	{
		if ( (++itArgs) != request.args.end() && (pos = containSemicol( *itArgs )) != std::string::npos )
		{
			if ( pos == 0 )
				return true;
			Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs));
			return false;
		}
		else if ( (++itArgs) != request.args.end() && (--request.args.end())->find(':') != 0)
			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
		else
			Server::sendToClient(client->socketFd, ERR_TOOMANYTARGETS);
	}
	return false;
}

void privmsgCmd(Client *client, const Request &request, Server *server) {

	if ( !requestIsValid( client, request ) )
		return ;
	std::string target = extractTarget( request );
	std::string messageToSend = extractMessage( request );
//	std::cerr << "target: '" << target << "'" << std::endl;
//	std::cerr << "mesage: '" << messageToSend << "'" << std::endl;
	if (targetIsChannel( target ))
	{
		messageToSend = RPL_CMD(client->nickName, client->userName, "PRIVMSG " + target , messageToSend);
		if (channelExist( server, client, target))
			sendMessageToAllChannelUsers( messageToSend, target, client);
		return ;
	}
	else if ( server->isUser( target ))
	{
		messageToSend = RPL_CMD(client->nickName, client->userName, "PRIVMSG", ": " + messageToSend);
		int clientFd = server->findUserSocketFd( target );
		Server::sendToClient(clientFd, messageToSend);
		return ;
	}
	Server::sendToClient(client->socketFd, ERR_NOSUCHNICK( client->nickName , target ));
}
