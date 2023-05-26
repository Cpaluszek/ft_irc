#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#list-message)
// Note: We do not manage ELIST!
void sendChannelInformation(Client *client, Channel *channel) {
	std::string channelName = channel->name;
	size_t clientCount = channel->getClientCount();
	std::stringstream s;
	s << clientCount;
	std::string clientCountStr = s.str();
	std::string channelTopic = channel->getTopic();
	// Note: if clientCount is 0
	Server::sendToClient(client->socketFd, RPL_LIST(client->nickName, channelName, clientCountStr, channelTopic));
}

void listCmd(Client *client, const Request &request, Server *server) {
	Server::channelMap channels = server->getChannels();
	Server::channelIt it;
	if (request.args.empty()) {
		// List all channels
		Server::sendToClient(client->socketFd, RPL_LISTSTART(client->nickName));
		for (it = channels.begin(); it != channels.end(); it++) {
			// If the channel is not secret 's'
			if (it->second->mode.find('s') == std::string::npos) {
				sendChannelInformation(client, it->second);
			}
		}
		Server::sendToClient(client->socketFd, RPL_LISTEND(client->nickName));
	}
	else {
		try {
			Server::channelIt it = channels.find(request.args[0]);
			sendChannelInformation(client, it->second);
		}
		catch (std::exception &e){
			std::cerr << "No such channel: " << request.args[0] << ". " << e.what() << std::endl;
		}
	}
}
