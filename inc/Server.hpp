/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:47:43 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/12 11:47:45 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <map>
#include <string>
#include <csignal>

#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include "Request.hpp"
#include "numericReplies.hpp"
#include "commands.hpp"
#include "colors.hpp"

// ----- SERVER INFO -----
#define LOCAL_HOST_IP "127.0.0.1"
#define SERVER_NAME std::string("FT_IRC")
#define SERVER_INFO std::string("This is an IRC server")
#define VERSION 	std::string("0.1")
#define MOTD_FILE 	"config/motd.txt"

#define CHANTYPES 	std::string("#")
#define PREFIX		std::string("@")
#define USERMODE 	std::string("ior")
#define CHANMODE 	std::string("itkol")
/* i - Set or remove invite-only channel
 * k - Set or remove the channel key
 * l - Set or remove the user limit to channel
 * n - Set or remove external user permission to send message to channel
 * o - Set or remove channel operator privilege
 * t - Set or remove the restriction of the TOPIC command to channel operators
 */

#define POLL_TIMEOUT 500
#define READ_BUFFER_SIZE 10000


#define CHANLIMIT 10  	// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#chanlimit-parameter)
#define CHANNELLEN 32 	// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#channellen-parameter)

// ---- RPL_ISUPPORT ----
// [RPL\_ISUPPORT Tokens](https://defs.ircdocs.horse/defs/isupport.html)
#define CHANLIMIT_TOKEN		std::string("CHANLIMIT=" + CHANTYPES + ":10")
#define CHANMODES_TOKEN		std::string("CHANMODES=" + CHANMODE)
#define CHANNELLEN_TOKEN 	std::string("CHANNELLEN=32")			// Maximum channel _name length
#define CHANTYPES_TOKEN		std::string("CHANTYPES=" + CHANTYPES)
#define KEYLEN_TOKEN		std::string("KEYLEN=10")				// Channel key length limit
#define KICKLEN_TOKEN		std::string("KICKLEN=")					// No length limit for kick message
#define MAXLIST_TOKEN		std::string("MAXLIST=")
#define MAXNICKLEN_TOKEN	std::string("MAXNICKLEN=32")			// Maximum nickname length
#define MAXTARGETS_TOKEN	std::string("MAXTARGETS=")
#define MODES_TOKEN			std::string("MODES=")
#define TARGMAX_TOKEN		std::string("TARGMAX=WHO:,WHOIS:1,NAMES:,PRIVMSG:,NOTICE:,JOIN:,PART:")
#define TOPICLEN_TOKEN		std::string("TOPICLEN=")				// Maximum topic length
#define PREFIX_TOKEN		std::string("PREFIX=" + PREFIX)
#define USERLEN_TOKEN		std::string("USERLEN=10")				// Maximum length of username
#define ISUPPORT_TOKEN 		std::string(CHANLIMIT_TOKEN + " " + CHANMODES_TOKEN + " " + CHANNELLEN_TOKEN \
		+ " " + CHANTYPES_TOKEN + " " + KEYLEN_TOKEN + " " + KICKLEN_TOKEN + " " + MAXLIST_TOKEN + " " \
  		+ MAXNICKLEN_TOKEN + " " + MAXTARGETS_TOKEN)

#define ISUPPORT_TOKEN2 	std::string(MODES_TOKEN + " " + TARGMAX_TOKEN + " " + TOPICLEN_TOKEN \
		+ " " + PREFIX_TOKEN + " " + USERLEN_TOKEN)
class Client;
class Channel;
class Request;

class Server {
public:
	static bool keyboardInterrupt;

	typedef void (*CmdFunction)(Client*, const Request &, Server*);
	typedef std::map<std::string, CmdFunction>::iterator cmdIt;
	typedef std::map<std::string, Channel*> channelMap;
	typedef std::map<std::string, Channel*>::iterator channelIt;
	typedef std::map<int, Client*> clientMap;
	typedef std::map<int, Client*>::iterator clientIt;
	typedef std::vector<std::string> vecStr;
	typedef std::vector<std::string>::iterator vecStrIt;

	~Server();
	Server(const std::string& port, const std::string& password);

	void 			Update();
	void 			sendWelcome(Client *client);
	bool 			isNickAlreadyUsed(const Client &client, std::string nick);
	void 			disconnectClient(Client *client);
	static void		handleKeyboardInterrupt(int signal);
	static void 	sendToClient(int fd, const std::string &content);

	std::string getPassword() const;
	clientMap 	getClients();

	// Channel
	bool		isAChannel(const std::string& channel);
	channelMap	getChannels();
	Channel 	*getChannelByName(const std::string& name);
	void		addChannel(Channel *newChannel);
	void 		removeChannel(const std::string &channelName);

	Client 		*getClientByNick(const std::string &nick);
	bool		isUser(const std::string &user);

private:
	int 							_serverSocketFd;
	unsigned int					_connectionCount;
	std::string 					_password;
	std::string 					_name;
	std::string 					_creationDate;
	std::map<int, Client*>			_clients;
	std::map<std::string, Channel*> _channels;
	std::map<std::string, CmdFunction> _commands;

	pollfd	*_pollFds;

	Server();
	void SetupServerSocket(int port);
	void initCommands();
	void registerNewClient();

	void readClientRequest(unsigned int index);
	void handleClientRequest(Client *client, const std::string& content);
};
