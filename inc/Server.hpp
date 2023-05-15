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
#include "Utils.hpp"
#include "Request.hpp"
#include "numericReplies.hpp"
#include "commands.hpp"
#include "colors.hpp"

#define LOCAL_HOST_IP "127.0.0.1"
#define SERVER_NAME std::string("FT_IRC")
#define VERSION std::string("0.1")
// Todo: USERMODE
#define USERMODE std::string("???")
// Todo: CHANMODE
#define CHANMODE std::string("???")

class Client;

class Server {
public:
	typedef void (*CmdFunction)(Client*, const Request &, Server*);
	typedef std::map<std::string, CmdFunction>::iterator cmdIt;
	std::string 			password;

	~Server();
	Server(std::string port, std::string password);
	void Update();
	static void sendToClient(int fd, const std::string &content);
	void sendWelcome(Client *client);
	bool isNickAlreadyUsed(const Client& client, std::string nick);
	void disconnectClient(int fd);

private:
	std::string 			_name;
	int 					_serverSocketFd;
	unsigned int			_connectionCount;
	std::map<int, Client>	_clients;
	std::string 			_creationDate;
	// map channels

	std::map<std::string, CmdFunction> _commands;

	// Note: Static allocation ? Or minimal allocation and realloc on new connections
	struct pollfd	*_pollFds;

	// Functions
	Server();
	void SetupServerSocket(int port);
	void registerNewClient();

	void readClientRequest(unsigned int index);
	void handleClientRequest(Client *client, const std::string& content);
};
