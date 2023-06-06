
#include "Server.hpp"
bool Server::keyboardInterrupt = false;

Server::Server(const std::string& port, const std::string& password) : _serverSocketFd(), _connectionCount(), _pollFds() {
	if (port.empty() || port.find_first_not_of("0123456789") != std::string::npos) {
		throw std::invalid_argument("Error: Wrong port format");
	}
	// Todo: add more protection

	std::istringstream cpp98Sucks(port);
	int portNumber;
	cpp98Sucks >> portNumber;
	if (cpp98Sucks.fail() || cpp98Sucks.eof() || portNumber < 1 || portNumber > 65535) {
		throw std::invalid_argument("Error: Wrong port format");
	}
	if (password.empty()) {
		throw std::invalid_argument("Error: Password cannot be empty");
	}
	this->_password = password;
	
	SetupServerSocket(portNumber);
	initCommands();

	this->_creationDate = Utils::getCurrentDateTime();
}

void Server::initCommands() {
	this->_commands["TOPIC"] = &topicCmd;
	this->_commands["PRIVMSG"] = &privmsgCmd;
	this->_commands["PASS"] = &passCmd;
	this->_commands["NICK"] = &nickCmd;
	this->_commands["USER"] = &userCmd;
	this->_commands["QUIT"] = &quitCmd;
	this->_commands["JOIN"] = &joinCmd;
	this->_commands["WHO"] = &whoCmd;
	this->_commands["PART"] = &partCmd;
	this->_commands["LIST"] = &listCmd;
	this->_commands["WHOIS"] = &whoisCmd;
	this->_commands["MOTD"] = &motdCmd;
	this->_commands["PING"] = &pingCmd;
	this->_commands["PART"] = &partCmd;
	this->_commands["NAMES"] = &namesCmd;
	this->_commands["AWAY"] = &awayCmd;
	this->_commands["KICK"] = &kickCmd;
	this->_commands["MODE"] = &mode;
	this->_commands["INVITE"] = &inviteCmd;
}

Server::~Server() {
	while (!this->_clients.empty()) {
		clientIt it = this->_clients.begin();
		it->second->leaveAllChannels();
		Server::sendToClient(it->second->socketFd, ERR_MSG(std::string("Server is shutting down")));
		disconnectClient(it->second);
	}
	close(this->_serverSocketFd);
	delete [] this->_pollFds;
	std::cout << BLUE << "[" << Utils::getCurrentDateTime() << "]" << RESET \
		<< GREEN << ": Server is shutting down" << RESET << std::endl;
}

void Server::SetupServerSocket(int port) {
	this->_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverSocketFd == -1) {
		throw std::runtime_error(std::string("Socket creation failed: ") + strerror(errno));
	}

	sockaddr_in address = {};
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
	this->_pollFds = new struct pollfd[SOMAXCONN];
	this->_pollFds[0].fd = this->_serverSocketFd;
	this->_pollFds[0].events = POLLIN;
	this->_connectionCount = 1;
	std::cout << BLUE << "[" << Utils::getCurrentDateTime() << "]" << RESET \
		<< GREEN << ": Server started ready to accept connections" << RESET << std::endl;
}

void Server::Update() {
	while (!Server::keyboardInterrupt) {
		int pollCount = poll(this->_pollFds, this->_connectionCount, POLL_TIMEOUT);
		if (pollCount == -1 && !Server::keyboardInterrupt) {
			throw std::runtime_error(std::string("poll() failed: ") + strerror(errno));
		}

		for (unsigned int i = 0; i < this->_connectionCount; i++) {
			if ((this->_pollFds[i].revents & POLLIN) == 0)
				continue;
			bool requestOnServerSocket = i == 0;
			requestOnServerSocket ? registerNewClient() : readClientRequest(i);
		}
	}
}

void Server::registerNewClient() {
	sockaddr_storage address = {};
	socklen_t addressLen = sizeof(address);
	int clientFd = accept(this->_serverSocketFd, (struct sockaddr*)&address, &addressLen);
	if (clientFd == -1) {
		throw std::runtime_error(std::string("accept() failed: ") + strerror(errno));
	}
	// Add new client to poll
	this->_pollFds[this->_connectionCount].fd = clientFd;
	this->_pollFds[this->_connectionCount].events = POLLIN;
	this->_pollFds[this->_connectionCount].revents = 0;
	this->_connectionCount += 1;

	// Add client to map
	this->_clients[clientFd] = new Client();
	this->_clients[clientFd]->socketFd = clientFd;

	std::cout << BLUE << "[" << Utils::getCurrentDateTime() << "]" << RESET << GREEN
			<< ": Connection on socket " << clientFd << RESET << std::endl;
}

void Server::disconnectClient(Client *client) {
	if (client == NULL) {
		std::cerr << RED << "Error: Client not found" << RESET << std::endl;
		return ;
	}
	int clientFd = client->socketFd;
	this->_clients.erase(clientFd);
	close(client->socketFd);
	delete client;

	for (unsigned int i = 0; i < this->_connectionCount; i++) {
		if (this->_pollFds[i].fd == clientFd) {
			this->_pollFds[i] = this->_pollFds[this->_connectionCount - 1];
			break ;
		}
	}
	this->_connectionCount -= 1;
}

void Server::readClientRequest(unsigned int index) {
	char buffer[READ_BUFFER_SIZE];
	memset(&buffer, 0, READ_BUFFER_SIZE);
	int clientFd = this->_pollFds[index].fd;
	Client *client = this->_clients[clientFd];

	ssize_t nBytes = recv(clientFd, buffer, sizeof(buffer), 0);

	if (nBytes <= 0) {
		if (nBytes < 0) {
			std::cout << "recv() error: " << strerror(errno) << std::endl;
		}
		std::cout << BLUE << "[" << Utils::getCurrentDateTime() << "]" << RESET << GREEN
			<< ": Disconnection on socket " << clientFd << RESET << std::endl;
		client->leaveAllChannels();
		disconnectClient(client);
		return ;
	}
	client->socketBuffer += std::string(buffer);
	size_t pos;
	while ((pos = client->socketBuffer.find("\r\n")) != std::string::npos) {
		std::string current = client->socketBuffer.substr(0, pos);
		client->socketBuffer.erase(0, pos + 2);
		handleClientRequest(client, current);
		if (this->_clients.find(clientFd) == this->_clients.end()) {
			break ;
		}
	}
}

void Server::handleClientRequest(Client *client, const std::string& content) {
	Request request(content);

#ifdef DEBUG
	std::cout << YELLOW << "# \"" << content << request;
#endif

	if (!request.isValid) {
		sendToClient(client->socketFd, "Invalid Message\n");
		return ;
	}
	cmdIt it = this->_commands.find(request.command);
	if (it != this->_commands.end()) {
		if (!client->hasMode('r') && request.command != "PASS" && request.command != "USER" && request.command != "NICK") {
			sendToClient(client->socketFd, ERR_NOTREGISTERED(client->nickName));
			return ;
		}
		it->second(client, request, this);
	}
	else if (request.command != "PONG" && request.command != "CAP") {
		sendToClient(client->socketFd, ERR_UNKNOWCOMMAND(client->nickName, request.command));
	}
}

bool Server::isNickAlreadyUsed(const Client& client, std::string nick) {
	std::string upperNick = Utils::copyToUpper(nick);
	std::transform(nick.begin(), nick.end(), nick.begin(), toupper);
	Server::clientIt it;
	for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (it->second->socketFd != client.socketFd && upperNick == Utils::copyToUpper(it->second->nickName)) {
			return true;
		}
	}
	return false;
}

void Server::sendWelcome(Client *client) {
	int fd = client->socketFd;
	sendToClient(fd, RPL_WELCOME(client->nickName, client->userName));
	sendToClient(fd, RPL_YOURHOST(client->nickName));
	sendToClient(fd, RPL_CREATED(client->nickName, _creationDate));
	sendToClient(fd, RPL_MYINFO(client->nickName));
	sendToClient(fd, RPL_ISUPPORT(client->nickName, ISUPPORT_TOKEN));
	sendToClient(fd, RPL_ISUPPORT(client->nickName, ISUPPORT_TOKEN2));
	motdCmd(client, Request(), this);
}

Channel *Server::getChannelByName(const std::string& name) {
	for (channelIt it = this->_channels.begin(); it != this->_channels.end(); it++) {
		if (it->second->getName() == name) {
			return it->second;
		}
	}
	return NULL;
}

void Server::addChannel(Channel *newChannel) {
	this->_channels[newChannel->getName()] = newChannel;
}

void Server::removeChannel(const std::string &channelName) {
	Server::channelIt it = this->_channels.find(channelName);
	if (it == this->_channels.end()) {
		std::cerr << RED << "Error: Channel not found" << RESET << std::endl;
		return ;
	}
	delete it->second;
	this->_channels.erase(it);
}

bool Server::isAChannel(const std::string& channel) {
	channelMap channels = this->_channels;
	if (channels.find( channel ) == channels.end())
		return false;
	return true;
}

bool Server::isUser( const std::string& user ) {
	clientIt it = this->_clients.begin();
	for (; it != this->_clients.end() ; it++) {
		if ( it->second->nickName == user )
			return true;
	}
	return false;
}

Server::channelMap Server::getChannels() {
	return this->_channels;
}

std::string Server::getPassword() const {
	return this->_password;
}

Client *Server::getClientByNick(const std::string &nick) {
	for (clientIt it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (Utils::copyToUpper(nick) == Utils::copyToUpper(it->second->nickName)) {
			return it->second;
		}
	}
	return NULL;
}

Server::clientMap Server::getClients() {
	return this->_clients;
}

Server::Server() {}

// ----- STATIC -----
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

void Server::handleKeyboardInterrupt(int signal) {
	std::cout << std::endl << RED << "Received signal: " << signal << RESET << std::endl;
	Server::keyboardInterrupt = true;
}
