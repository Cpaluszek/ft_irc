#include "Client.hpp"

Client::Client(): socketFd(0), isRegistered(false), hasPassword(false), socketBuffer("") {
}

Client::~Client() {
}

void Client::updateNickname(const std::string& newNickname) {
	this->previousNickname = this->nickName;
	this->nickName = newNickname;
}

std::ostream &operator<<(std::ostream &out, const Client &src) {
	out << "Client: socket(" << src.socketFd << ") - nickname(" << src.nickName <<") - username(" << src.userName \
		<< ") - isRegistered(" << src.isRegistered <<") - hasPassword(" << src.hasPassword <<")" <<std::endl;
	return out;
}
