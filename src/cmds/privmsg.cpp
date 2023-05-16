#include "commands.hpp"

std::vector<std::string> parsPrivMsg( Client *client, const Request &request )
{
	size_t pos = -1;
	std::vector<std::string> userAndMessage;
	std::vector<std::string>::const_iterator itArgs = request.args.begin();
	std::vector<std::string>::const_iterator itForUser = request.args.begin();

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
				if ( itArgs->length() != 1 )
					userAndMessage.push_back( itArgs->substr( pos + 1, itArgs->length() ));
				else if ( (itArgs + 1) == request.args.end() )
				{
					Server::sendToClient(client->socketFd, ERR_NOTEXTTOSEND(client->nickName));
					return userAndMessage ;
				}
				while ( itForUser != request.args.end() && itForUser->find(":") == std::string::npos )
				{
					userAndMessage.push_back( *itForUser );
					itForUser++;
				}
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
	return userAndMessage;
}

bool	targetExist( std::string target, std::string nickname )
{
	Server::clientIt it = Server::getClientBeginIt();
	Server::clientIt itEnd = Server::getClientBeginIt();
	for (; it != itEnd ; it++) {
		if ( it->second.nickName == target )
			return true;
	}
	Server::sendToClient(ERR_NOSUCHNICK( , target ))
	return false;
}
// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#privmsg-message)
void privmsgCmd(Client *client, const Request &request, Server *server) {
	(void) server;
	(void) request;
	(void) client;

	std::vector<std::string> userAndMessage = parsPrivMsg( client, request );
	if (userAndMessage.empty())
		return ;
	std::vector<std::string>::iterator it = userAndMessage.begin();
	if (targetExist( it[0] , client->nickName ))
	{
		Server::sendToClient()
	}



	std::cout << "PRIVMSG not implemented" << std::endl;
}
