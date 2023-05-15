#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#quit-message)
void quitCmd(Client *client, const Request &request, Server *server) {
	(void) server;
	(void) request;
	(void) client;
	// Todo: remove client from map
	// Todo: leave all channels that the user is in
	// Todo: close the client socket and remove from poll
	std::cout << "QUIT not implemented" << std::endl;
}
