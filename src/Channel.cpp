#include "Channel.hpp"

Channel::Channel(const std::string& name, Client *client, Server *server): name(name), symbol('='),  _server(server), _topic("") {
	this->mode = "t";
	channelUser newClient;
	newClient.client = client;
	newClient.prefix = "@";
	newClient.userMode = "o";
	newClient.joinTime = Utils::getCurrentDateTime();
	this->_mapClients[client->nickName] = newClient;
	_creationTime = Utils::getCurrentDateTime();
}

Channel::Channel() {}

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

Channel::mapClients Channel::getClients() const {
	return this->_mapClients;
}

std::string Channel::getTopic() const {
	return this->_topic;
}

std::string Channel::getTopicUser() const {
	return this->_topicSetBy;
}

std::string Channel::getTopicTime() const {
	return this->_topicSetTime;
}

void Channel::addClient(Client *client) {
	channelUser newClient;
	newClient.client = client;
	newClient.prefix = "";
	newClient.userMode = "";
	newClient.joinTime = Utils::getCurrentDateTime();
	this->_mapClients[client->nickName] = newClient;
}

void Channel::setTopic(const std::string &newTopic, const std::string &nick) {
	this->_topic = newTopic;
	this->_topicSetBy = nick;
	this->_topicSetTime = Utils::getCurrentDateTime();
}

bool Channel::isClientConnected(const std::string& nickName) const {
	return this->_mapClients.find(nickName) != this->_mapClients.end();
}

size_t Channel::getClientCount() const {
	return this->_mapClients.size();
}

void Channel::eraseClient(const std::string& client) {
	this->_mapClients.erase(client);
	if (this->getClientCount() == 0) {
		_server->removeChannel(this->name);
	}
}

void Channel::sendToAllclient(std::string message) {
	mapClientsIt it;

	it = this->_mapClients.begin();
	for (; it != this->_mapClients.end(); it++){
		Server::sendToClient(it->second.client->socketFd,message);
	}
}

void Channel::sendToAllclientExceptSender(std::string message, Client *client) {
	mapClientsIt it;

	it = this->_mapClients.begin();
	for (; it != this->_mapClients.end(); it++){
		if ( it->first != client->nickName )
			Server::sendToClient(it->second.client->socketFd,message);
	}
}

void Channel::updateTopic( const std::string &newTopic, const std::string &setBy ) {
	if (newTopic.empty())
		this->_topic.clear();
	else
		setTopic(newTopic, setBy);
}

