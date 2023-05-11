#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#user-message)
void userCmd(Client *client, const Request &request, Server *server) {
	(void) client;
	(void) request;
	(void) server;
	std::cout << "USER not implemented" << std::endl;
}
