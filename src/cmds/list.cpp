#include "commands.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#list-message)

/*   LIST                            ; Command to list all channels
*
*  LIST #twilight_zone,#42         ; Command to list the channels
*                                  "#twilight_zone" and "#42".
*
*  LIST >3                         ; Command to list all channels with
*                                  more than three users.
*
*  LIST C>60                       ; Command to list all channels with
*                                  created at least 60 minutes ago
*
*  LIST T<60                       ; Command to list all channels with
*                                  a topic changed within the last 60 minutes
*/
// Note: do we have to manage ELIST ? Conditions for LIST -> [IRC Client Protocol Specification](https://modern.ircdocs.horse/#elist-parameter)

void sendChannelInformation(Client *client, Channel *channel) {
	std::string channelName = channel->name;
	size_t clientCount = channel->getClientCount();
	std::stringstream s;
	s << clientCount;
	std::string clientCountStr = s.str();
	std::string channelTopic = channel->getTopic();
	Server::sendToClient(client->socketFd, RPL_LIST(client->nickName, channelName, clientCountStr, channelTopic));
}

void listCmd(Client *client, const Request &request, Server *server) {
	Server::channelMap channels = server->getChannels();
	Server::channelIt it;
	if (request.args.empty()) {
		// List all channels
		Server::sendToClient(client->socketFd, RPL_LISTSTART(client->nickName));
		for (it = channels.begin(); it != channels.end(); it++) {
			sendChannelInformation(client, it->second);
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
		// Find the requested channel

	}
}
