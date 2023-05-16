#pragma once

#include <iostream>

#include "Client.hpp"
#include "Utils.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#channels)

class Client;

typedef struct t_connectedClient
{
	Client *client;
	std::string prefix;
	std::string joinTime;
	std::string userMode;
} connectedClient;

// Todo: founder + operator
class Channel {
public:
	typedef std::map<std::string, connectedClient> mapClients;
	typedef std::map<std::string, connectedClient>::iterator mapClientsIt;

	// Todo: use getters and setters  + private vars
	std::string name;
	std::string topic;
	std::string mode;
	char symbol;

	Channel(const std::string& name, Client *client);
	Channel();

	std::string getPrefix(const std::string& clientName);
	std::string getKey() const;

	// Todo: addClient

	mapClients getClients() const;
	void addClient(Client *client);
	void removeClient(Client *client);
	// Todo: removeClient

private:
	// Todo: instead of Client - use a struct with joinTime userMode prefix
	mapClients _mapClients;
	std::string _creationTime;
	std::string _key;

	// Functions:
		// Connect client
		// Disconnect client

};

