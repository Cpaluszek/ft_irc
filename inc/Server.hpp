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
#include "NumericReplies.hpp"

#define LOCAL_HOST_IP "127.0.0.1"
#define SERVER_NAME std::string("FT_IRC")
#define WELCOME_MSG "----- $%@^&*(#(_!(#*)&#))*% -----\n"

#ifndef DEBUG_RESPONSE
	#define DEBUG_RESPONSE
#endif

class Client;

class Server {
public:
	~Server();
	Server(std::string port, std::string password);
	void Update();

private:
	Server();
	void SetupServerSocket(int port);
	void registerNewClient();
	void readClientRequest(unsigned int index);
	void sendToClient(int fd, std::string content);
	std::string handleClientRequest(std::string request, int fd);

	// Commands
	std::string passCmd(const Request& request, int fd);

	std::string 			_name;
	std::string 			_password;
	int 					_serverSocketFd;
	unsigned int			_connectionCount;
	std::map<int, Client>	_clients;

	// Note: Static allocation ? Or minimal allocation and realloc on new connections
	// Todo: map ou list
	struct pollfd	*_pollFds;
};
