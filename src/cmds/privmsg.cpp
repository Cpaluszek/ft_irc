#include "commands.hpp"

//send to all users in channel except source client
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

//std::vector<std::string> parsPrivMsg( Client *client, const Request &request )
//{
//	size_t pos;
//	std::vector<std::string> userAndMessage;
//	std::vector<std::string>::const_iterator itArgs = request.args.begin();
//	std::vector<std::string>::const_iterator itForUser = request.args.begin();
//
//	for (; itArgs != request.args.end(); ++itArgs) {
//		pos = itArgs->find(":");
//		if (pos == std::string::npos && (itArgs) == request.args.begin() && (itArgs + 1) == request.args.end())
//		{
//			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
//			break;
//		}
//		else if (pos == std::string::npos && (itArgs) == request.args.begin())
//			continue ;
//		else if (pos == std::string::npos && (itArgs - 1) == request.args.begin())
//		{
//			Server::sendToClient(client->socketFd, ERR_TOOMANYTARGETS);
//			break;
//		}
//		if (pos != std::string::npos && pos == 0)
//		{
//			if ((itArgs) == request.args.begin())
//			{
//				Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
//				break;
//			}
//			else if ( (itArgs - 1 ) == request.args.begin() )
//			{
//				while ( itForUser != request.args.end() && itForUser->find(":") == std::string::npos )
//				{
//					userAndMessage.push_back( *itForUser );
//					itForUser++;
//				}
//				if ( itArgs->length() != 1 )
//				{
//					userAndMessage.push_back();
//					std::cerr << userAndMessage[1];
//				}
//				else if ( (itArgs + 1) == request.args.end() )
//				{
//					Server::sendToClient(client->socketFd, ERR_NOTEXTTOSEND(client->nickName));
//					return userAndMessage ;
//				}
//				break;
//			}
//		}
//		else if ( pos != std::string::npos)
//		{
//			Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs));
//			break ;
//		}
//		else {
//			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
//			break;
//		}
//	}
//	return userAndMessage;
//}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#privmsg-message)

size_t 	containSemicol( std::string arg )
{
	size_t pos = arg.find( ":" );
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
	if ( (pos = containSemicol( *itArgs )) != std::string::npos )
	{
		if ( pos == 0 )
			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
		else
			Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs));
		return false;
	}
	else
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

std::string extractTarget( const Request &request )
{
	return (*request.args.begin());
}

std::string extractMessage( const Request &request )
{
	std::string message = *(++request.args.begin());
	return (message.substr(1, message.length()));
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
		messageToSend = RPL_CMD(client->nickName, client->userName, "PRIVMSG", ": " + messageToSend);
		if (channelExist( server, client, target))
			sendMessageToAllChannelUsers( messageToSend, target, client);
		return ;
	}
	else if ( server->isUser( target ))
	{
		int clientFd = server->findUserSocketFd( target );
		Server::sendToClient(clientFd, messageToSend);
		return ;
	}
	Server::sendToClient(client->socketFd, ERR_NOSUCHNICK( client->nickName , target ));
}
