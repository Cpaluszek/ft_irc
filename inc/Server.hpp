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
#include "Channel.hpp"
#include "Utils.hpp"
#include "Request.hpp"
#include "numericReplies.hpp"
#include "commands.hpp"
#include "colors.hpp"

#define LOCAL_HOST_IP "127.0.0.1"
#define SERVER_NAME std::string("FT_IRC")
#define SERVER_INFO std::string("This is an IRC server")
#define VERSION std::string("0.1")

// Note: do we need to add 'w' - wallops
// USERMODE: invisible(i) - oper(o) - registered(r)
// Todo: use usermode 'r' for register instead of bool
#define USERMODE std::string("iorw")
// Todo: CHANMODE
#define CHANMODE std::string("???")

#define MOTD_FILE "config/motd.txt"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#chanlimit-parameter)
#define CHANLIMIT 10

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#channellen-parameter)
#define CHANNELLEN 32

class Client;
class Channel;
class Request;

class Server {
public:
	typedef void (*CmdFunction)(Client*, const Request &, Server*);
	typedef std::map<std::string, CmdFunction>::iterator cmdIt;
	typedef std::map<std::string, Channel*> channelMap;
	typedef std::map<std::string, Channel*>::iterator channelIt;
	typedef std::map<int, Client*> clientMap;
	typedef std::map<int, Client*>::iterator clientIt;
	// Todo: Check for usage
	typedef std::vector<std::string> vecStr;
	typedef std::vector<std::string>::iterator vecStrIt;

	// Todo: Make password private with a getter and no setter

	~Server();
	Server(std::string port, const std::string& password);
	void 		Update();
	void 		sendWelcome(Client *client);
	bool 		isNickAlreadyUsed(const Client &client, std::string nick);
	void 		disconnectClient(int fd);

	std::string getPassword() const;
	clientIt getClientBeginIt();
	clientIt getClientEndIt();
	clientMap getClients();
	// Channel
	bool		isAChannel(const std::string& channel);
	channelMap	getChannels();
	channelIt	getChannelByName(const std::string& name);
	channelIt	getChannelEnd(); // a remove?
	bool		isAChannel( const std::string &channel ) const;
	void		addChannel(Channel *newChannel);
	void 		removeChannel(const std::string &channelName);

	static void sendToClient(int fd, const std::string &content);
	Client 		*getClientByNick(const std::string &nick);
	int			findUserSocketFd(const std::string &user);
	bool		isUser(const std::string &user);

protected:
	std::string 				_password;
	std::string 				_name;
	int 						_serverSocketFd;		// Note: create a class module for socket management?
	unsigned int				_connectionCount;
	std::map<int, Client*>		_clients;
	std::string 				_creationDate;
	std::map<std::string, Channel*> _channels;
	// map channels

	std::map<std::string, CmdFunction> _commands;

	// Note: Static allocation ? Or minimal allocation and realloc on new connections
	pollfd	*_pollFds;

	// Functions
	Server();
	void SetupServerSocket(int port);
	void registerNewClient();

	void readClientRequest(unsigned int index);
	void handleClientRequest(Client *client, const std::string& content);

};
