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

#define LOCAL_HOST_IP "127.0.0.1"
#define WELCOME_MSG "$%@^&*(#(_!(#*)&#))*%"

// Note: use singleton pattern -> seems not useful
class Server {
public:
	~Server();
	Server(std::string port, std::string password);
	void serverLoop(void);

private:
	Server();
	void SetupServerSocket(int port);
	void registerNewClient();
	void handleClientRequest(unsigned int index);

	static std::string getCurrentDateTime();

	std::string 	_name;
	std::string 	_password;
	int 			_serverSocketFd;
	unsigned int	_connectionCount;

	// Note: Static allocation ? Or minimal allocation and realloc on new connections
	struct pollfd	*_pollFds;
};