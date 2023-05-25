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

bool Client::isOnChannel(std::string channel) const {
	if (_channels.find(channel) == _channels.end())
		return false;
	return true;
}

std::string Client::getMode() const {
	return this->_mode;
}

void Client::eraseChannel(std::string channel) {
	this->_channels.erase(channel);
}

void	Client::quit() {

	channelMapIt it = this->_channels.begin();
	for(; it != this->_channels.end(); it++){
	Channel *channel = it->second;
	channel->eraseClient(this->nickName);
	channel->sendToAllclient(RPL_CMD(this->nickName, this->userName, "PART", (channel->name + " has quit")));
}

}

std::ostream &operator<<(std::ostream &out, const Client &src) {
	out << "Client: socket(" << src.socketFd << ") - nickname(" << src.nickName <<") - username(" << src.userName \
		<< ") - isRegistered(" << src.isRegistered <<") - hasPassword(" << src.hasPassword <<")" <<std::endl;
	return out;
}
