#pragma once

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <map>
#include <string>

#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include "Request.hpp"
#include "numericReplies.hpp"
#include "commands.hpp"
#include "colors.hpp"

#define LOCAL_HOST_IP "127.0.0.1"
#define SERVER_NAME std::string("FT_IRC")
#define VERSION std::string("0.1")
#define MOTD_FILE "config/motd.txt"
// Todo: USERMODE
#define USERMODE std::string("???")
// Todo: CHANMODE
#define CHANMODE std::string("???")

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#chanlimit-parameter)
#define CHANLIMIT 10

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#channellen-parameter)
#define CHANNELLEN 32

class Client;
class Channel;

class Server {
public:
	typedef void (*CmdFunction)(Client*, const Request &, Server*);
	typedef std::map<std::string, CmdFunction>::iterator cmdIt;
	typedef std::map<std::string, Channel*>::iterator channelIt;
	typedef std::map<int, Client>::iterator clientIt;
	// Todo: typedef vecStr iterator

	~Server();
	Server(const std::string& port, const std::string& password);
	void 		Update();
	void 		sendWelcome(Client *client);
	bool 		isNickAlreadyUsed(const Client& client, std::string nick);
	void 		disconnectClient(int fd);

	std::string getPassword() const;
	clientIt getClientBeginIt();
	clientIt getClientEndIt();

	// Channel
	channelIt	getChannelByName(const std::string& name);
	channelIt	getChannelEnd();
	void		addChannel(Channel *newChannel);
	// Todo: remove channel

	static void sendToClient(int fd, const std::string &content);

private:
	std::string 				_name;
	std::string 				_password;
	int 						_serverSocketFd;		// Note: create a class module for socket management?
	unsigned int				_connectionCount;
	std::map<int, Client>		_clients;	// Note: convert to map<int, Client*> ????
	std::string 				_creationDate;
	std::map<std::string, Channel*> _channels;
	// map channels

	std::map<std::string, CmdFunction> _commands;

	// Note: Static allocation ? Or minimal allocation and realloc on new connections
	// Todo: map ou list
	struct pollfd	*_pollFds;

	// Functions
	Server();
	void SetupServerSocket(int port);
	void registerNewClient();

	void readClientRequest(unsigned int index);
	void handleClientRequest(Client *client, const std::string& content);
};
