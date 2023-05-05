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

#define LOCAL_HOST_IP "127.0.0.1"

// Note: use singleton pattern?
class Server {
public:
	~Server();
	Server(std::string port, std::string password);

private:
	Server();
	void SetupServerSocket(int port);


	std::string 	_name;
	std::string 	_password;
	int 			_serverSocketFd;
	unsigned int	_connectionCount;
};