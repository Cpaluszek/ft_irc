#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#user-message)
/**
 * Command: USER
 * 		Parameters: <username> 0 * <realname>
 *
 * Example:
 * USER guest 0 * :Ronnie Reagan
 * 	- user get registered with username "guest" and real name "Ronnie Reagan"
 */
void userCmd(Client *client, const Request &request, Server *server) {
	(void) server;
	if (request.args.empty() || request.args.size() < 3){
		Server::sendToClient(client->socketFd, ERR_NEEDMOREPARAMS(client->nickName, std::string("USER")));
	}
	else if (!client->hasPassword) {
		Server::sendToClient(client->socketFd, ERR_MSG(std::string("PASS is needed")));
	}
	else if (client->isRegistered) {
		Server::sendToClient(client->socketFd, ERR_ALREADYREGISTERED(client->nickName));
	}
	else {
		client->userName = request.args[0];
		if (request.args.size() > 3) {
			client->realName = request.args[3];
		}
		if (!client->nickName.empty()) {
			client->isRegistered = true;
			Server::sendToClient(client->socketFd, WELCOME_MSG);
		}
	}
}
