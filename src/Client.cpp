#include "Client.hpp"

Client::Client(): socketFd(0), isRegistered(false), hasPassword(false) {
}

Client::~Client() {
}

void Client::updateNickname(std::string newNickname) {
	this->previousNickname = this->nickName;
	this->nickName = newNickname;
}

