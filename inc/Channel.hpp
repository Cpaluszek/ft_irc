#pragma once

#include <iostream>

#include "Client.hpp"
#include "Utils.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#channels)

class Client;
class Server;


/*  ----- CHANNEL MODES -----
 * i - Set or remove invite-only channel
 * k - Set or remove the channel key
 * l - Set or remove the user limit to channel
 * o - Set or remove channel operator privilege
 * t - Set or remove the restriction of the TOPIC command to channel operators
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
	// Todo: switch a map of ChannelUser*
	typedef std::map<std::string, channelUser> mapClients;
	typedef std::map<std::string, channelUser>::iterator mapClientsIt;

	// Todo: use getters and setters  + private vars
	std::string name;
	std::string mode;
	char symbol;

	Channel(const std::string& name, Client *client, Server *server);
	Channel();

	std::string getMode() const;
	bool hasMode(char c) const;
	std::string getPrefix(const std::string& clientName);
	std::string getKey() const;
	std::string getTopic() const;
	std::string getTopicUser() const;
	std::string getTopicTime() const;


	void updateTopic( const std::string &newTopic, const std::string &setBy );

	void setTopic(const std::string &newTopic, const std::string &nick);

	mapClients	getClients() const;
	size_t		getClientCount() const;
	bool 		isClientConnected(const std::string& nickName) const;
	void		sendToAllclient(std::string message);
	void		sendToAllclientExceptSender(std::string message, Client *client);
	channelUser * getChannelUserByNick(const std::string &nick);
	void		addClient(Client *client);
	void		eraseClient(const std::string& client);

	// INVITE LIST
	void addInvite(const std::string &nickName);
	void removeInvite(const std::string &nickName);
	bool isInvited(const std::string &nickName) const;

private:
	Server *_server;
	mapClients _mapClients;
	std::string _creationTime;
	std::string _key;
	std::string _topic;
	std::string _topicSetBy;
	std::string _topicSetTime;
	std::vector<std::string> _inviteList;
};

