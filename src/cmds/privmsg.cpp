#include "commands.hpp"
#include "Utils.hpp"

bool	targetIsChannel( std::string target )
{
	if ( target[0] == '#')
		return true;
	return false;
}

static bool	channelExist( Server *server , Client *client, const std::string& channel )
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

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#privmsg-message)
bool	Request::requestPrivMsgIsValid(Client *client) const//, const Request &request
{
	if (this->args.empty()) {
		Server::sendToClient( client->socketFd, ERR_NORECIPIENT( client->nickName, request.command));
		return false;
	}
	size_t pos;
	std::vector<std::string>::const_iterator itArgs = this->args.begin();
	if ((pos = Utils::getSemicolonPos(*itArgs)) != std::string::npos ) // If ":" is in first arg
	{
		if ( pos == 0 )
			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
		else
			Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs));
	}
	else //If ":" is not in first arg
	{
		if ( (++itArgs) != this->args.end() && (pos = Utils::getSemicolonPos(*itArgs)) != std::string::npos )
		{
			if ( pos == 0 )
			{
				if ( itArgs->length() > 1 || ++itArgs != this->args.end() )
					return true;
				Server::sendToClient(client->socketFd, ERR_NOTEXTTOSEND(client->nickName));
			}
			Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs));
		}
		else if ( (++itArgs) != this->args.end() && (--this->args.end())->find(':') != 0)
			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
		else
			Server::sendToClient(client->socketFd, ERR_TOOMANYTARGETS);
	}
	return false;
}

void privmsgCmd(Client *client, const Request &request, Server *server) {

	if ( !request.requestPrivMsgIsValid( client ) )
		return ;
	std::string target = extractTarget( request );
	std::string messageToSend = extractMessage( request );
//	std::cerr << "target: '" << target << "'" << std::endl;
//	std::cerr << "mesage: '" << messageToSend << "'" << std::endl;
	if (targetIsChannel( target ))
	{
		if (channelExist( server, client, target)) //TODO: check if user is on channel
		{
			Channel *specificChannel = server->getChannelByName( target )->second;
			messageToSend = RPL_CMD(client->nickName, client->userName, "PRIVMSG " + target , messageToSend);

			specificChannel->sendToAllclientExceptSender( messageToSend, client );
		}
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
