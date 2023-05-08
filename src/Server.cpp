
#include "../inc/Server.hpp"

Server::Server(std::string port, std::string password) {
	std::cout << "Port: " << port << std::endl;
// Todo: how to detect port argument error ?
// numbers only - range [0, ???]
	SetupServerSocket(atoi(port.c_str()));

	// Todo: set server password
	if (password.empty()) {
		throw std::invalid_argument("Password cannot be empty");
	}
	this->_password = password;
	std::cout << "Password: " << password << std::endl;

	// Setup poll file descriptors
	this->_pollFds = new struct pollfd[SOMAXCONN];
	this->_pollFds[0].fd = this->_serverSocketFd;
	this->_pollFds[0].events = POLLIN;

	this->_connectionCount = 1;
}

Server::~Server() {
	close(this->_serverSocketFd);
	if (this->_pollFds) {	// Note: unnecessary delete protection?
		delete [] this->_pollFds;
	}
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
		throw std::runtime_error(std::string("setsockopt() failed: ") + strerror(errno));
	}

	if (bind(this->_serverSocketFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		throw std::runtime_error(std::string("bind() failed: ") + strerror(errno));
	}

	if (listen(this->_serverSocketFd, SOMAXCONN) < 0) {
		throw std::runtime_error(std::string("listen() failed: ") + strerror(errno));
	}

	std::cout << "Server started ready to accept connections" << std::endl;
}

void Server::serverLoop(void) {
	int pollCount;
	while (true) {
		pollCount = poll(this->_pollFds, this->_connectionCount, -1);
		if (pollCount == -1) {
			throw std::runtime_error(std::string("poll() failed: ") + strerror(errno));
		}

		for (unsigned int i = 0; i < this->_connectionCount; i++) {
			if ((this->_pollFds[i].revents & POLLIN) == 0) {
				continue;
			}
			if (this->_pollFds[i].fd == this->_serverSocketFd) {
				// New client
				std::cout << "New client connection" << std::endl;
				registerNewClient();
			}
			else {
				// Client request
				std::cout << "Client request" << std::endl;
				handleClientRequest(i);
			}
		}
	}
}

void Server::registerNewClient() {
	// Todo: sockaddr_in vs sockaddr_storage?
	struct sockaddr_in address;
	socklen_t addressLen = sizeof(address);
	int clientFd = accept(this->_serverSocketFd, (struct sockaddr*)&address, &addressLen);
	if (clientFd == -1) {
		throw std::runtime_error(std::string("accept() failed: ") + strerror(errno));
	}
	// Add new client to poll
	this->_pollFds[this->_connectionCount].fd = clientFd;
	this->_pollFds[this->_connectionCount].events = POLLIN;
	this->_connectionCount += 1;

	// Todo: add client to map

	// Send welcome message
	if (send(clientFd, WELCOME_MSG, std::string(WELCOME_MSG).length(), 0) == -1) {
		std::cout << "send() error: " << strerror(errno) << std::endl;
	}
	std::cout << "[" << getCurrentDateTime() << "]: new connection from " << inet_ntoa(address.sin_addr) << " on socket " << clientFd << std::endl;
}

// Note: rename to readClientRequest() ?
void Server::handleClientRequest(unsigned int index) {
	// Note: what size to use ?
	char buffer[10000];
	ssize_t nBytes = recv(this->_pollFds[index].fd, buffer, sizeof(buffer), 0);

	if (nBytes <= 0) {
		if (nBytes < 0) {
			std::cout << "recv() error: " << strerror(errno) << std::endl;
		}
		std::cout << "[" << getCurrentDateTime() << "]: socket " << this->_pollFds[index].fd << " disconnected" << std::endl;
		close(this->_pollFds[index].fd);
		this->_pollFds[index] = this->_pollFds[this->_connectionCount - 1];
		// Todo: remove from map
		this->_connectionCount -= 1;
	}
	else {
		std::cout << "Content: " << buffer << std::endl;
		// Todo: handle client request and respond
	}

	memset(&buffer, 0, 6000);
}


// ------------ STATIC ---------------
// Note: move to another class? Utils class?
std::string Server::getCurrentDateTime() {
	time_t     now = time(0);
	char       buf[80];

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", localtime(&now));
	return buf;
}
