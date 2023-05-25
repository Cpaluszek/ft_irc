#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#ping-message)
void pingCmd(Client *client, const Request &request, Server *server) {
	(void) server;
	if (request.args.empty()) {
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, request.command));
		return ;
	}
	Server::sendToClient(client->socketFd, RPL_CMD(client->nickName, client->userName, \
			std::string("PONG"), SERVER_NAME + " " + request.args[0]));
}
