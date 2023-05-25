#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#pass-message)
void passCmd(Client *client, const Request &request, Server *server) {
	if (client->hasPassword) {
		return ;
	}
	if (client->isRegistered) {
		Server::sendToClient(client->socketFd, ERR_ALREADYREGISTERED(client->nickName));
	}
	else if (request.args.empty() || request.args[0].length() == 0) {
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, std::string("PASS")));
	}
	else if (request.args.size() == 1 && request.args[0] == server->password) {
		// Note: used for nick and user too?
		client->hasPassword = true;
	}
	else {
		// Todo: Disconnect client ? + send message to announce disconnection?
		Server::sendToClient(client->socketFd, ERR_PASSWDMISMATCH(std::string("Client")));
	}
}

