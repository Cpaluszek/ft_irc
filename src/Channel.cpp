#include "Channel.hpp"

Channel::Channel(const std::string& name, Client *client, Server *server): _server(server), _name(name), _symbol('='), _topic("") {
	this->_mode = "nt";
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
		_server->removeChannel(this->_name);
	}
}

void Channel::updateClient(const std::string &oldNick, const std::string &newNick) {
	mapClientsIt it = this->_mapClients.find(oldNick);
	if (it != this->_mapClients.end()) {
		channelUser temp = it->second;
		this->_mapClients.erase(it);
		this->_mapClients[newNick] = temp;
	}
}
void Channel::sendToAllClients(const std::string& message) {
	if (this->_mapClients.empty()) {
		return ;
	}
	mapClientsIt it;
	it = this->_mapClients.begin();
	for (; it != this->_mapClients.end(); it++){
		Server::sendToClient(it->second.client->socketFd,message);
	}
}

void Channel::sendToAllClientsExceptSender(const std::string& message, Client *client) {
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

std::string Channel::getMode() const {
    return this->_mode;
}

void Channel::addMode(char c) {
    if (this->_mode.find(c) == std::string::npos) {
        this->_mode += c;
    }
}

void Channel::removeMode(char c) {
    size_t pos = this->_mode.find(c);
    if (pos != std::string::npos) {
        this->_mode.erase(pos);
    }
}

channelUser * Channel::getChannelUserByNick(const std::string &nick) {
	for (mapClientsIt it = this->_mapClients.begin(); it != this->_mapClients.end(); it++) {
		if (Utils::copyToUpper(nick) == Utils::copyToUpper(it->second.client->nickName)) {
			return &it->second;
		}
	}
	return NULL;
}


void Channel::setClientLimit(const std::string& limit) {
	// Todo: verify that the limit is >0
    this->_clientLimit = atoi(limit.c_str());
}

size_t Channel::getClientLimit() const {
    return this->_clientLimit;
}

bool Channel::hasMode(char c) const {
	return this->_mode.find(c) != std::string::npos;
}

void Channel::addInvite(const std::string &nickName) {
	if (isInvited(nickName))
		return ;
	this->_inviteList.push_back(nickName);
}

void Channel::removeInvite(const std::string &nickName) {
	std::vector<std::string>::iterator it;
	for (it = this->_inviteList.begin(); it != this->_inviteList.end(); it++) {
		if (*it == nickName) {
			this->_inviteList.erase(it);
			break ;
		}
	}
}

bool Channel::isInvited(const std::string &nickName) const {
	std::vector<std::string>::const_iterator it;
	for (it = this->_inviteList.begin(); it != this->_inviteList.end(); it++) {
		if (*it == nickName) {
			return true;
		}
	}
	return false;
}

std::string Channel::getCreationTime() {
	return this->_creationTime;
}

bool Channel::isClientOperator(std::string &nickName) {
	std::string mode = this->getChannelUserByNick( nickName )->userMode;
	if ( !mode.empty() && mode.find('o') != std::string::npos )
		return true;
	return false;
}

std::string Channel::getName() const {
	return this->_name;
}

char Channel::getSymbol() const {
	return this->_symbol;
}

void Channel::setKey(std::string &key) {
    this->_key = key;
}
