
#include "Server.hpp"

Server::Server(std::string port, std::string password) {
	// Todo: how to detect port argument error ? numbers only - range [0, 65535]
	SetupServerSocket(atoi(port.c_str()));

	if (password.empty()) {
		throw std::invalid_argument("Password cannot be empty");
	}
	this->password = password;

	// Setup poll file descriptors
	this->_pollFds = new struct pollfd[SOMAXCONN];
	this->_pollFds[0].fd = this->_serverSocketFd;
	this->_pollFds[0].events = POLLIN;

	this->_connectionCount = 1;

	// Init Commands
	this->_commands["PASS"] = &passCmd;
	this->_commands["NICK"] = &nickCmd;
	this->_commands["USER"] = &userCmd;
	this->_commands["QUIT"] = &quitCmd;

	this->_creationDate = Utils::getCurrentDateTime();
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
	std::cout << BLUE << "[" << Utils::getCurrentDateTime() << "]" << RESET \
		<< GREEN << ": Server started ready to accept connections" << RESET << std::endl;
}

void Server::Update() {
	int	pollCount = poll(this->_pollFds, this->_connectionCount, -1);
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
	this->_clients[clientFd].socketFd = clientFd;

	std::cout << BLUE << "[" << Utils::getCurrentDateTime() << "]" << RESET << GREEN
			<< ": Connection on socket " << clientFd << RESET << std::endl;
}

void Server::readClientRequest(unsigned int index) {
	// Note: what size to use ?
	char buffer[10000];
	memset(&buffer, 0, 10000);
	ssize_t nBytes = recv(this->_pollFds[index].fd, buffer, sizeof(buffer), 0);

	if (nBytes <= 0) {
		if (nBytes < 0) {
			std::cout << "recv() error: " << strerror(errno) << std::endl;
		}
		std::cout << BLUE << "[" << Utils::getCurrentDateTime() << "]" << RESET << GREEN
			<< ": Disconnection on socket " << this->_pollFds[index].fd << RESET << std::endl;
		this->_clients.erase(this->_pollFds[index].fd);
		close(this->_pollFds[index].fd);
		this->_pollFds[index] = this->_pollFds[this->_connectionCount - 1];
		this->_connectionCount -= 1;
	}
	else {
		Client *client = &this->_clients[this->_pollFds[index].fd];
		client->socketBuffer += std::string(buffer);
		size_t pos;
		while ((pos = client->socketBuffer.find("\r\n")) != std::string::npos) {
			std::string current = client->socketBuffer.substr(0, pos);
			client->socketBuffer.erase(0, pos + 2);
			handleClientRequest(client, current);
		}
	}
}

void Server::sendToClient(int fd, const std::string &content) {
#ifdef DEBUG
	std::cout << CYAN << "->" << content << RESET << std::endl;
#endif
	size_t bytesSent = 0;
	while (bytesSent < content.length()) {
		ssize_t len = send(fd, content.c_str(), content.length(), 0);
		if (len < 0) {
			std::cout << "send() error: " << strerror(errno) << std::endl;
			break ;
		}
		bytesSent += len;
	}
}

void Server::handleClientRequest(Client *client, const std::string& content) {
	Request request(content);

#ifdef DEBUG
	std::cout << YELLOW << "# \"" << content << request;
#endif

	if (!request.isValid) {
		// Note: how to manage invalid messages?
		sendToClient(client->socketFd, "Invalid Message\n");
		return ;
	}
	cmdIt it = this->_commands.find(request.command);
	if (it != this->_commands.end()) {
		it->second(client, request, this);
	}
#ifdef DEBUG
	else {
		std::cout << CYAN << "->Unknown command: " << content << RESET << std::endl;
	}
#endif
}

bool Server::isNickAlreadyUsed(const Client& client, std::string nick) {
	std::string upperNick = Utils::copyToUpper(nick);
	std::transform(nick.begin(), nick.end(), nick.begin(), toupper);
	std::map<int, Client>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (it->second.socketFd != client.socketFd && upperNick == Utils::copyToUpper(it->second.nickName)) {
			return true;
		}
	}
	return false;
}

void Server::sendWelcome(Client *client) {
	int fd = client->socketFd;
	sendToClient(fd, RPL_WELCOME(client->nickName, client->userName, std::string(LOCAL_HOST_IP)));
	sendToClient(fd, RPL_YOURHOST(client->nickName));
	sendToClient(fd, RPL_CREATED(client->nickName, _creationDate));
	sendToClient(fd, RPL_MYINFO(client->nickName));
	// Note: RPL_ISSUPPORT ?? -> OSKOUR
	// Note: MOTD?
	// Note: mode?
}
