#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#privmsg-message)
void privmsgCmd(Client *client, const Request &request, Server *server) {
	(void) server;
	(void) request;
	(void) client;

	// Todo: ERR_NORECIPIENT
	// Todo: ERR_NOTEXTTOSEND
	// Todo: ERR_NOSUCHNICK
	// Todo: send priv msg to target
	std::cout << "PRIVMSG not implemented" << std::endl;
}
