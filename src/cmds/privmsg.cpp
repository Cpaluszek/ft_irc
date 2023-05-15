#include "commands.hpp"

std::string parsPrivMsg( Client *client, const Request &request )
{
	size_t pos = -1;
	std::string user = "";
	std::vector<std::string>::const_iterator itArgs = request.args.begin();
	std::vector<std::string>::const_iterator it = request.args.begin();


	for (; it != request.args.end(); ++it) {
		std::cerr << *it << std::endl;
	}
	for (; itArgs != request.args.end(); ++itArgs) {
		pos = itArgs->find(":");
		if (pos != std::string::npos)
		{
			if (itArgs != request.args.begin() && (itArgs - 1) != request.args.begin() && pos != 0)
			{
				Server::sendToClient(client->socketFd, ERR_TOOMANYTARGETS);
				break;
			}
			else if ((itArgs - 1) != request.args.begin() && pos == 0)
			{
				Server::sendToClient(client->socketFd, ERR_NORECIPIENT(client->nickName, request->command));
				break;
			}
			else if ((pos == 0 && (itArgs + 1) == request.args.end() && request.args.size() == 0) || \
					(pos == itArgs->length() - 1 && (itArgs + 1) == request.args.end()))
			{
				Server::sendToClient(client->socketFd, ERR_NOTEXTTOSEND(client->nickName));
				break ;
			}
			else if (pos != 0 && (itArgs - 1) == request.args.begin())
			{
				user = itArgs->substr(0, pos);
				break ;
			}
		}
		else if ((itArgs - 1) != request.args.begin())
		{
			Server::sendToClient(client->socketFd, ERR_TOOMANYTARGETS);
			break ;
		}
	}
	return user;
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#privmsg-message)
void privmsgCmd(Client *client, const Request &request, Server *server) {
	(void) server;
	(void) request;
	(void) client;
	std::string clientNickname;
	std::string originalMessageToSend;


	clientNickname = parsPrivMsg( client, request );
	if (clientNickname.empty())
		return ;

// Todo: ERR_NOTEXTTOSEND
//	parsPrivMsg( pos, itArgs );
	// Todo: ERR_NOSUCHNICK
	// Todo: send priv msg to target
	std::cout << "PRIVMSG not implemented" << std::endl;
}
