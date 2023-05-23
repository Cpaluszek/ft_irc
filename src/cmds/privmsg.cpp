#include "commands.hpp"

//send to all users in channel except source client
void	sendMessageToAllChannelUsers( std::string message, std::string channel, Client *client)
{
	(void) message;
	std::map<std::string, Channel*> channelsMap= client->getChannels();
	std::map<std::string, Channel*>::iterator it = channelsMap.find( channel );
	if (it != channelsMap.end())
		std::cerr << it->second->getClients().begin()->first << std::endl;
	std::map<std::string, t_channelUser> mapChannelUsers = it->second->getClients();
	std::map<std::string, t_channelUser>::iterator itMapChannelUsers = mapChannelUsers.begin();
	for (; itMapChannelUsers != mapChannelUsers.end() ; ++itMapChannelUsers) {
		{
			if ( itMapChannelUsers->second.client->nickName != client->nickName)
				Server::sendToClient(itMapChannelUsers->second.client->socketFd, message);
		}
	}
}

bool	targetIsChannel( std::string target )
{
	if ( target[0] == '#')
		return true;
	return false;
}

bool	channelExist( Server *server , Client *client, std::string channel )
{
	std::map<std::string, Channel*>::iterator channelsMapIt = server->getChannelByName( channel );
	if (channelsMapIt != server->getChannelEnd())
		return true;
	Server::sendToClient(client->socketFd, ERR_NOSUCHNICK(client->nickName, channel));
	return false;
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
	// Todo: use server->isNickAlreadyUsed() instead
	Server::clientIt it = server->getClientBeginIt();
	Server::clientIt itEnd = server->getClientEndIt();
	std::map<int, bool> existAndClientFd;

	for (; it != itEnd ; it++) {
		if ( it->second->nickName == target )
		{
			existAndClientFd[ it->second->socketFd ] = true;
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
	if (userAndMessage.empty() || userAndMessage.size() != 2)
		return ;

	if (targetIsChannel( userAndMessage[0] ))
	{
		std::string channel = userAndMessage[0].substr(1, userAndMessage[0].length());
		if (channelExist( server, client, channel))
			sendMessageToAllChannelUsers( userAndMessage[1], userAndMessage[0], client);
		return ;
	}
	else
	{
		std::map<int, bool> clientFdAndExist = targetExist( server, userAndMessage[0], client);

		if (clientFdAndExist.begin()->second)
		{
			int clientfd = clientFdAndExist.begin()->first;
			Server::sendToClient(clientfd, userAndMessage[1]);
		}
	}

}
