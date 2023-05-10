
#include "../inc/Server.hpp"

Server::Server(std::string port, std::string password) {
	std::cout << "Port: " << port << std::endl;
	// Todo: how to detect port argument error ? numbers only - range [0, ???]
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
	// Note: unnecessary delete protection?
	if (this->_pollFds) {
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
	std::cout << "[" << Utils::getCurrentDateTime() << "]: Server started ready to accept connections" << std::endl;
}

void Server::serverLoop() {
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
				registerNewClient();
			}
			else {
				readClientRequest(i);
			}
		}
	}
}

void Server::registerNewClient() {
	struct sockaddr_storage address;
	socklen_t addressLen = sizeof(address);
	int clientFd = accept(this->_serverSocketFd, (struct sockaddr*)&address, &addressLen);
	if (clientFd == -1) {
		throw std::runtime_error(std::string("accept() failed: ") + strerror(errno));
	}
	// Add new client to poll
	this->_pollFds[this->_connectionCount].fd = clientFd;
	this->_pollFds[this->_connectionCount].events = POLLIN;
	this->_connectionCount += 1;

	// Add client to map
	this->_clients[clientFd];

	// Send welcome message
	if (send(clientFd, WELCOME_MSG, std::string(WELCOME_MSG).length(), 0) == -1) {
		std::cout << "send() error: " << strerror(errno) << std::endl;
	}
	std::cout << "[" << Utils::getCurrentDateTime() << "]: new connection from " << inet_ntoa(((struct sockaddr_in *)&address)->sin_addr)
			<< " on socket " << clientFd << std::endl;
}

void Server::readClientRequest(unsigned int index) {
	// Note: what size to use ?
	char buffer[10000];
	ssize_t nBytes = recv(this->_pollFds[index].fd, buffer, sizeof(buffer), 0);

	if (nBytes <= 0) {
		if (nBytes < 0) {
			std::cout << "recv() error: " << strerror(errno) << std::endl;
		}
		std::cout << "[" << Utils::getCurrentDateTime() << "]: socket " << this->_pollFds[index].fd << " disconnected" << std::endl;
		this->_clients.erase(this->_pollFds[index].fd);
		close(this->_pollFds[index].fd);
		// TODO: attention au trash code
		this->_pollFds[index] = this->_pollFds[this->_connectionCount - 1];
		this->_connectionCount -= 1;
	}
	else {
		// Todo: Check for '\r\n'
		std::string input(buffer);
		size_t pos;
		while ((pos = input.find("\r\n")) != std::string::npos) {
			std::string current = input.substr(0, pos);
			input.erase(0, pos + 2);
			std::string response = handleClientRequest(current, this->_pollFds[index].fd);
			if (response.length() == 0) {
				continue ;
			}
			if (send(this->_pollFds[index].fd, response.c_str(), response.length(), 0) == -1) {
				std::cout << "send() error: " << strerror(errno) << std::endl;
			}
		}
	}
	memset(&buffer, 0, 10000);
}

// Todo: if forest alternative
std::string Server::handleClientRequest(std::string rawString, int fd) {
	Request request(rawString);
	if (!request.isValid) {
		return "Invalid Message\n";
	}
	if (request.command == "CAP") {
		// Note: ignore CAP ?
		return NO_RESPONSE;
	}
	if (request.command == "PASS") {
		return passCmd(request, fd);
	}
	if (request.command == "NICK") {
		return "NICK command\n";
	}
	if (request.command == "USER") {
		return "USER command\n";
	}
	return "Invalid command\n";
}

// --------------------------------------- COMMANDS -----------------------------------------------------------------//
// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#pass-message)
std::string Server::passCmd(const Request& request, int fd) {
	if (this->_clients[fd].isRegistered) {
		return ERR_ALREADYREGISTERED(this->_clients[fd].nickName);
	}
	if (request.args.empty()) {
		return ERR_NEEDMOREPARAMS(this->_clients[fd].nickName, std::string("PASS"));
	}
	if (request.args.size() == 1 && request.args[0] == this->_password) {
		this->_clients[fd].isRegistered = true;
		return NO_RESPONSE;
	}
	return ERR_PASSWDMISMATCH(this->_clients[fd].nickName);
}

