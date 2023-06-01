#pragma once

#include <iostream>

#include "Client.hpp"
#include "Utils.hpp"
# define ADDMOD		200
# define REMOVMOD 	201

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#channels)

class Client;
class Server;


/*  ----- CHANNEL MODES -----
 * b - bans
 * i - invite necessary
 * k - key is needed
 * l - limit number of users
 * n - users outside the channel can NOT send PRIVMSG - TODO: ???
 * o - operator status
 * s - secret channel - TODO: ???
 * t - only operator can change the topic
 */

typedef struct t_channelUser
{
	Client *client;
	std::string prefix;
	std::string joinTime;
	std::string userMode;
} channelUser;

// Todo: founder + operator
class Channel {
public:
	typedef std::map<std::string, channelUser> mapClients;
	typedef std::map<std::string, channelUser>::iterator mapClientsIt;

	// Todo: use getters and setters  + private vars
	std::string name;

	char symbol;

	Channel(const std::string& name, Client *client, Server *server);
	Channel();

	std::string getPrefix(const std::string& clientName);
	std::string getKey() const;
	std::string getTopic() const;
	std::string getTopicUser() const;
	std::string getTopicTime() const;


	void updateTopic( const std::string &newTopic, const std::string &setBy );

	void setTopic(const std::string &newTopic, const std::string &nick);

	mapClients		getClients() const;
	size_t			getClientCount() const;
	bool 			isClientConnected(const std::string& nickName) const;
	void			sendToAllclient(std::string message);
	void			sendToAllclientExceptSender(std::string message, Client *client);

	void			addClient(Client *client);
	void			eraseClient(const std::string& client);

	// ------- Mode -------
	std::string		getMods();
	void 			setMods( std::string mod, int action );


private:
	Server *_server;
	mapClients 	_mapClients;
	std::string _creationTime;
	std::string _key;
	std::string _topic;
	std::string _topicSetBy;
	std::string _topicSetTime;
	std::string	mode;
};
