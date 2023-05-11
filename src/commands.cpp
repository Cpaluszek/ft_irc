#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#pass-message)
void passCmd(Client client, const Request &request, Server *server) {
	if (client.isRegistered) {
		Server::sendToClient(client.socketFd, ERR_ALREADYREGISTERED(client.nickName));
	}
	if (request.args.empty() || request.args[0].length() == 0) {
		Server::sendToClient(client.socketFd, ERR_NEEDMOREPARAMS(client.nickName, std::string("PASS")));
	}
	if (request.args.size() == 1 && request.args[0] == server->password) {
		client.isRegistered = true;
	}
	// Todo: Disconnect client ? + send message to announce disconnection?
	Server::sendToClient(client.socketFd, ERR_PASSWDMISMATCH(std::string("Client")));
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#nick-message)
void nickCmd(Client client, const Request &request, Server *server) {
	(void) client;
	(void) request;
	(void) server;
	std::cout << "NICK not implemented" << std::endl;
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#user-message)
void userCmd(Client client, const Request &request, Server *server) {
	(void) client;
	(void) request;
	(void) server;
	std::cout << "USER not implemented" << std::endl;
}
