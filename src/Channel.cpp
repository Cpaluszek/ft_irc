#include "Channel.hpp"

Channel::Channel(const std::string& name, Client *client): name(name), topic(""), symbol('=') {
	connectedClient newClient;
	newClient.client = client;
	newClient.prefix = "@";
	newClient.userMode = "o";
	newClient.joinTime = Utils::getCurrentDateTime();
	this->_mapClients[client->nickName] = newClient;
	_creationTime = Utils::getCurrentDateTime();
}

Channel::Channel() {
}

std::string Channel::getPrefix(const std::string& clientName) {
	mapClientsIt it;
	for (it = this->_mapClients.begin(); it != this->_mapClients.end(); it++) {
		if (it->second.client->nickName == clientName) {
			return it->second.prefix;
		}
	}
	return "";
}

std::string Channel::getKey() const {
	return this->_key;
}

void Channel::addClient(Client *client) {
	connectedClient newClient;
	newClient.client = client;
	newClient.prefix = "";
	newClient.userMode = "";
	newClient.joinTime = Utils::getCurrentDateTime();
	this->_mapClients[client->nickName] = newClient;
}

void Channel::removeClient(Client *client) {
	mapClientsIt it = this->_mapClients.find(client->nickName);
	if (it != this->_mapClients.end()) {
		this->_mapClients.erase(it);
	}
}

Channel::mapClients Channel::getClients() const {
	return this->_mapClients;
}
