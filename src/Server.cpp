
#include "../inc/Server.hpp"

Server::Server(std::string port, std::string password) {
	this->_password = password;
	this->_connectionCount = 0;
	std::cout << "Port: " << port << std::endl;
	std::cout << "Password: " << password << std::endl;
// Todo: how to detect port argument error ?
	SetupServerSocket(atoi(port.c_str()));
}

Server::~Server() {
	close(this->_serverSocketFd);
}

void Server::SetupServerSocket(int port) {
	// Todo: should we use getaddrinfo ? -> https://github.dev/barimehdi77/ft_irc/tree/main/srcs

	this->_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverSocketFd == -1) {
		throw std::runtime_error(std::string("Socket creation failed: ") + strerror(errno));
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(LOCAL_HOST_IP);

	int opt = 1;
	if (setsockopt(this->_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		throw std::runtime_error(std::string("setsockopt failed: ") + strerror(errno));
	}

	if (bind(this->_serverSocketFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		throw std::runtime_error(std::string("Bind failed: ") + strerror(errno));
	}

	if (listen(this->_serverSocketFd, SOMAXCONN) < 0) {
		throw std::runtime_error(std::string("Listen failed: ") + strerror(errno));
	}

	std::cout << "Server started" << std::endl;
}
