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
	std::cout << "USER not implemented" << std::endl;
//	if (request.args.empty() || request.args[0].length() < 1){
//
//	}
	(void) server;
	(void) request;
	if (client->isRegistered) {
		Server::sendToClient(client->socketFd, ERR_ALREADYREGISTERED(client->nickName));
	}
}
