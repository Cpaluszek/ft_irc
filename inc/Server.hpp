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

#include "Client.hpp"
#include "Utils.hpp"
#include "Request.hpp"

#define LOCAL_HOST_IP "127.0.0.1"
#define WELCOME_MSG "----- $%@^&*(#(_!(#*)&#))*%\n -----"

class Client;

// Note: use singleton pattern -> seems not useful
// Todo: how to use the password ? -> on new client connection?

class Server {
public:
	~Server();
	Server(std::string port, std::string password);
	void serverLoop();

private:
	Server();
	void SetupServerSocket(int port);
	void registerNewClient();
	void readClientRequest(unsigned int index);
	std::string handleClientRequest(std::string request, int index);

	std::string 			_name;
	std::string 			_password;
	int 					_serverSocketFd;
	unsigned int			_connectionCount;
	std::map<int, Client>	_clients;

	// Note: Static allocation ? Or minimal allocation and realloc on new connections
	// Todo: map ou list
	struct pollfd	*_pollFds;
};
