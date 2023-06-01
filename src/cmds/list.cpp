#include "commands.hpp"

void sendChannelInformation(Client *client, Channel *channel) {
	std::string channelName = channel->name;
	size_t clientCount = channel->getClientCount();
	std::stringstream s;
	s << clientCount;
	std::string clientCountStr = s.str();
	std::string channelTopic = channel->getTopic();
	Server::sendToClient(client->socketFd, RPL_LIST(client->nickName, channelName, clientCountStr, channelTopic));
}

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#list-message)
void listCmd(Client *client, const Request &request, Server *server) {
	Server::channelMap channels = server->getChannels();
	Server::channelIt it;
	if (request.args.empty()) {
		Server::sendToClient(client->socketFd, RPL_LISTSTART(client->nickName));
		for (it = channels.begin(); it != channels.end(); it++) {
			// If the channel is not secret 's'
			if (it->second->getMods().find('s') == std::string::npos) {
				sendChannelInformation(client, it->second);
			}
		}
		Server::sendToClient(client->socketFd, RPL_LISTEND(client->nickName));
		return ;
	}
	Server::channelIt channelIt = channels.find(request.args[0]);
	if (channelIt == server->getChannelEnd())
		std::cerr << "No such channel: " << request.args[0] << std::endl;
	else
		sendChannelInformation(client, channelIt->second);
}
