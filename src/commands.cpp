#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#pass-message)
std::string passCmd(Client client, const Request &request, Server *server) {
	if (client.isRegistered) {
		return ERR_ALREADYREGISTERED(client.nickName);
	}
	if (request.args.empty() || request.args[0].length() == 0) {
		return ERR_NEEDMOREPARAMS(client.nickName, std::string("PASS"));
	}
	if (request.args.size() == 1 && request.args[0] == server->password) {
		client.isRegistered = true;
		return NO_RESPONSE;
	}
	// Todo: Disconnect client ? + send message to announce disconnection?
	return ERR_PASSWDMISMATCH(std::string("Client"));
}
