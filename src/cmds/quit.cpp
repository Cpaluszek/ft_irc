#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#quit-message)
void quitCmd(Client *client, const Request &request, Server *server) {
	(void) server;
	(void) request;
	(void) client;
	std::cout << "QUIT not implemented" << std::endl;
}
