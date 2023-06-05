#include "Client.hpp"

Client::Client(): socketFd(0), hasPassword(false), isAway(false), socketBuffer("") {
}

Client::~Client() {
}

void Client::updateNickname(const std::string& newNickname) {
	this->previousNickname = this->nickName;
	this->nickName = newNickname;
}

void Client::addChannel(Channel *newChannel) {
	this->_channels[newChannel->getName()] = newChannel;
}

size_t Client::getNumberOfChannelsJoined() const {
	return this->_channels.size();
}

Client::channelMap Client::getChannels() const {
	return this->_channels;
}

bool Client::isOnChannel(const std::string& channel) const {
	if (_channels.find(channel) == _channels.end())
		return false;
	return true;
}


void Client::eraseChannel(const std::string& channel) {
	this->_channels.erase(channel);
}

void	Client::leaveAllChannels() {
	channelMapIt it = this->_channels.begin();
	for(; it != this->_channels.end(); it++) {
		Channel *channel = it->second;
		channel->sendToAllClients(RPL_CMD(this->nickName, this->userName, "PART", (it->first + " leave all channels")));
		channel->eraseClient(this->nickName);
	}
}

std::string Client::getMode() const {
	return this->_mode;
}

bool Client::hasMode(char c) const {
	return this->_mode.find(c) != std::string::npos;
}

void Client::addMode(char c) {
	if (this->_mode.find(c) == std::string::npos) {
		this->_mode += c;
	}
}

void Client::removeMode(char c) {
	size_t pos = this->_mode.find(c);
	if (pos != std::string::npos) {
		this->_mode.erase(pos);
	}
}

std::ostream &operator<<(std::ostream &out, const Client &src) {
	out << "Client: socket(" << src.socketFd << ") - nickname(" << src.nickName <<") - username(" << src.userName \
		<< ") - mode(" << src.getMode() <<") - hasPassword(" << src.hasPassword <<")" <<std::endl;
	return out;
}
