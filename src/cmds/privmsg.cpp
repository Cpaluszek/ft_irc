#include "commands.hpp"

bool	targetIsChannel( std::string target )
{
	if ( target[0] == '#')
		return true;
	return false;
}

bool channelExist( std::string channel )
{
	if ()
}

std::vector<std::string> parsPrivMsg( Client *client, const Request &request )
{
	size_t pos;
	std::vector<std::string> userAndMessage;
	std::vector<std::string>::const_iterator itArgs = request.args.begin();
	std::vector<std::string>::const_iterator itForUser = request.args.begin();

	for (; itArgs != request.args.end(); ++itArgs) {
		pos = itArgs->find(":");
		if (pos == std::string::npos && (itArgs) == request.args.begin() && (itArgs + 1) == request.args.end())
		{
			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
			break;
		}
		else if (pos == std::string::npos && (itArgs) == request.args.begin())
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
				while ( itForUser != request.args.end() && itForUser->find(":") == std::string::npos )
				{
					userAndMessage.push_back( *itForUser );
					itForUser++;
				}
				if ( itArgs->length() != 1 )
				{
					userAndMessage.push_back(RPL_CMD(client->nickName, client->userName, "PRIVMSG", ": " + itArgs->substr(pos + 1, itArgs->length())));
					std::cerr << userAndMessage[1];
				}
				else if ( (itArgs + 1) == request.args.end() )
				{
					Server::sendToClient(client->socketFd, ERR_NOTEXTTOSEND(client->nickName));
					return userAndMessage ;
				}
				break;
			}
		}
		else if ( pos != std::string::npos)
		{
			Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, *itArgs));
			break ;
		}
		else {
			Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request.command));
			break;
		}
	}
	return userAndMessage;
}

std::map<int, bool>	targetExist( Server *server, std::string target, Client *client )
{
	Server::clientIt it = server->getClientBeginIt();
	Server::clientIt itEnd = server->getClientEndIt();
	std::map<int, bool> existAndClientFd;

	for (; it != itEnd ; it++) {
		if ( it->second.nickName == target )
		{
			existAndClientFd[ it->second.socketFd ] = true;
			return (existAndClientFd);
		}
	}
	Server::sendToClient(client->socketFd, ERR_NOSUCHNICK( client->nickName , target ));
	existAndClientFd[ -1 ] = false;
	return existAndClientFd;
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#privmsg-message)
void privmsgCmd(Client *client, const Request &request, Server *server) {

	if (request.args.empty()) {
		Server::sendToClient( client->socketFd, ERR_NORECIPIENT( client->nickName, request.command));
		return ;
	}
	std::vector<std::string> userAndMessage = parsPrivMsg( client, request );
	if (userAndMessage.empty())
		return ;

	std::vector<std::string>::iterator it = userAndMessage.begin();
	if (targetIsChannel && channelExist)
	{
		sendMessageToAllChannelUsers();
	}
	std::map<int, bool> clientFdAndExist = targetExist( server, it[0], client);

	if (clientFdAndExist.begin()->second)
	{
		int clientfd = clientFdAndExist.begin()->first;
		Server::sendToClient(clientfd, userAndMessage[1]);
	}
}
