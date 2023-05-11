#include "commands.hpp"

bool containsInvalidChars(std::string nick) {
	return (nick.find_first_of(" ,*?!@.") != std::string::npos ||
	 nick[0] == '$' || nick[0] == ':' || nick[0] == '#' || nick[0] == '&');
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#nick-message)
void nickCmd(Client *client, const Request &request, Server *server) {
	if (!client->hasPassword) {
		std::cout << "Nick: " << client->hasPassword << std::endl;
		Server::sendToClient(client->socketFd, ERR_MSG(std::string("PASS is needed")));
		return ;
	}
	// No nickname given
	std::string nick = request.args[0];
	if (request.args.empty() || request.args[0].length() == 0) {
		Server::sendToClient(client->socketFd, ERR_NONICKNAMEGIVEN(std::string("Client")));
	}
	// Invalid characters
	else if (containsInvalidChars(nick)) {
		Server::sendToClient(client->socketFd, ERR_ERRONEUSNICKNAME(nick));
	}
	// Nickname already used
	else if (server->isNickAlreadyUsed(*client, nick)) {
		Server::sendToClient(client->socketFd, ERR_NICKNAMEINUSE(nick));
	}
	// Success
	else {
		client->updateNickname(nick);
		if (!client->isRegistered && client->userName.length() != 0) {  // First nickname input
			client->isRegistered = true;
			Server::sendToClient(client->socketFd, WELCOME_MSG);
		}
		else { // Update nickname
			Server::sendToClient(client->socketFd, RPL_NICK(client->previousNickname, nick, client->userName, std::string(LOCAL_HOST_IP)));
			// TODO: update nick in each channel information
				// find client.channels
#ifdef DEBUG
			std::cout << "Nickname update: " << client->previousNickname << "->" << client->nickName << std::endl;
#endif
		}
	}
}
