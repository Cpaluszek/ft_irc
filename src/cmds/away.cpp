#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#away-message)
void awayCmd(Client *client, const Request &request, Server *server) {
	(void) server;
	if (request.args.empty() || request.args[0].empty()) {
		if (client->isAway) {
			client->isAway = false;
			Server::sendToClient(client->socketFd, RPL_UNAWAY(client->nickName));
		}
	}
	else if (!client->isAway){
		client->awayMessage = request.args[0];
		client->isAway = true;
		Server::sendToClient(client->socketFd, RPL_NOWAWAY(client->nickName));
	}
}
