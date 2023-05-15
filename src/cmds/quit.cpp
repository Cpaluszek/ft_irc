#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#quit-message)
void quitCmd(Client *client, const Request &request, Server *server) {
	(void) request;
	Server::sendToClient(client->socketFd, ERR_MSG(std::string("Disconnected from server")));
	server->disconnectClient(client->socketFd);
	// Todo: leave all channels that the user is in
}
