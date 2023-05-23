#include "Client.hpp"

Client::Client(): socketFd(0), isRegistered(false), hasPassword(false), socketBuffer("") {
}

Client::~Client() {
}

void Client::updateNickname(const std::string& newNickname) {
	this->previousNickname = this->nickName;
	this->nickName = newNickname;
}

void Client::addChannel(Channel *newChannel) {
	this->_channels[newChannel->name] = newChannel;
}

size_t Client::getNumberOfChannelsJoined() const {
	return this->_channels.size();
}

Client::channelMap Client::getChannels() const {
	return this->_channels;
}

std::string Client::getMode() const {
	return this->_mode;
}

bool Client::isOnChannel(const std::string &channel) {
	if ( this->_channels.find( channel ) == this->_channels.end() )
		return false;
	return true;
}

std::ostream &operator<<(std::ostream &out, const Client &src) {
	out << "Client: socket(" << src.socketFd << ") - nickname(" << src.nickName <<") - username(" << src.userName \
		<< ") - isRegistered(" << src.isRegistered <<") - hasPassword(" << src.hasPassword <<")" <<std::endl;
	return out;
}
