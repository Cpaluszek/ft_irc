#include "commands.hpp"

std::vector<std::string> parsPrivMsg( Client *client, const Request &request )
{
	size_t pos = -1;
	std::vector<std::string> userAndMessage;
	std::vector<std::string>::const_iterator itArgs = request.args.begin();

	for (; itArgs != request.args.end(); ++itArgs) {
		pos = itArgs->find(":");
		if (pos == std::string::npos && (itArgs) == request.args.begin())
			continue ;
		else if (pos == std::string::npos && (itArgs - 1) == request.args.begin())
		{
			Server::sendToClient(client->socketFd, ERR_TOOMANYTARGETS);
			break;
		}
		if (pos != std::string::npos && pos == 0)
		{
			if ((itArgs) == request.args.begin())
			{
				Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
				break;
			}
			else if ( (itArgs - 1 ) == request.args.begin() )
			{
				userAndMessage.push_back( *(itArgs) );
				break;
			}
		}
		else if ( pos != std::string::npos && pos != 0)
		{
			Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs));
			break ;
		}
		else if (pos == std::string::npos)
		{
			std::cerr << "ok" <<std::endl;

			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
			break;
		}
	}
	if (!userAndMessage.empty())
	{
		if ( itArgs->length() != 1 )
			userAndMessage.push_back( itArgs->substr( pos, itArgs->length() - pos ));
		else if ( (itArgs + 1) == request.args.end() )
		{
			Server::sendToClient(client->socketFd, ERR_NOTEXTTOSEND(client->nickName));
			return userAndMessage ;
		}
		while (++itArgs != request.args.end())
		{
			userAndMessage.push_back( *itArgs );
			itArgs++;
		}
	}
	return userAndMessage;
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#privmsg-message)
void privmsgCmd(Client *client, const Request &request, Server *server) {
	(void) server;
	(void) request;
	(void) client;

	std::vector<std::string> userAndMessage = parsPrivMsg( client, request );
	if (userAndMessage.empty())
		return ;

// Todo: ERR_NOTEXTTOSEND
//	parsPrivMsg( pos, itArgs );
	// Todo: ERR_NOSUCHNICK
	// Todo: send priv msg to target
	std::cout << "PRIVMSG not implemented" << std::endl;
}
