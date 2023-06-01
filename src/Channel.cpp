#include "Channel.hpp"

Channel::Channel(const std::string& name, Client *client, Server *server): name(name), symbol('='),  _server(server), _topic("") {
	this->mode = "nt";
	channelUser newClient;
	newClient.client = client;
	newClient.prefix = "@";
	newClient.userMode = "o";
	newClient.joinTime = Utils::getCurrentDateTime();
	// Todo: we need to use to upper in nicknames
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

void Channel::updateClient(const std::string &oldNick, const std::string &newNick) {
	mapClientsIt it = this->_mapClients.find(oldNick);
	if (it != this->_mapClients.end()) {
		t_channelUser temp = it->second;
		this->_mapClients.erase(it);
		this->_mapClients[newNick] = temp;
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

channelUser * Channel::getChannelUserByNick(const std::string &nick) {
	for (mapClientsIt it = this->_mapClients.begin(); it != this->_mapClients.end(); it++) {
		if (Utils::copyToUpper(nick) == Utils::copyToUpper(it->second.client->nickName)) {
			return &it->second;
		}
	}
	return NULL;
}

void Channel::setMods(std::string mod, int action) {
	switch (action) {
		case ADDMOD:
			for (size_t i = 0; i < mod.length(); ++i) {
				if ( this->mode.find(mod[i]) == std::string::npos )
					this->mode += mod[i];
			}
			break;
		case REMOVMOD:
		{
			std::string newMod;
			for (size_t i = 0; i < this->mode.length(); ++i) {
				if ( mod.find(this->mode[i]) == std::string::npos )
					newMod += this->mode[i];
			}
			this->mode = newMod;
			break;
		}
		default:
			break;
	}
}

std::string Channel::getMods() {
	return this->mode;
}

bool Channel::hasMode(char c) const {
	return this->mode.find(c) != std::string::npos;
}
void Channel::addInvite(const std::string &nickName) {
	if (isInvited(nickName))
		return ;
	this->_inviteList.push_back(nickName);
}

// Todo: need to convert nickname to uppercase??
void Channel::removeInvite(const std::string &nickName) {
	std::vector<std::string>::iterator it;
	for (it = this->_inviteList.begin(); it != this->_inviteList.end(); it++) {
		if (*it == nickName) {
			this->_inviteList.erase(it);
			break ;
		}
	}
}

// Todo: need to convert nickname to uppercase??
bool Channel::isInvited(const std::string &nickName) const {
	std::vector<std::string>::const_iterator it;
	for (it = this->_inviteList.begin(); it != this->_inviteList.end(); it++) {
		if (*it == nickName) {
			return true;
		}
	}
	return false;
}


