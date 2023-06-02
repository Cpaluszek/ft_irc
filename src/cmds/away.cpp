#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#away-message)
void awayCmd(Client *client, const Request &request, Server *server) {
	// Todo: PRIVMSG check for AWAY -> RPL_AWAY
	(void) server;
	if (request.args.empty() || request.args[0].empty()) {
		// No longer away
		if (client->isAway) {
			client->isAway = false;
			Server::sendToClient(client->socketFd, RPL_UNAWAY(client->nickName));
		}
	}
	else if (!client->isAway){
		// Marked away
		client->awayMessage = request.args[0];
		client->isAway = true;
		Server::sendToClient(client->socketFd, RPL_NOWAWAY(client->nickName));
	}
}
