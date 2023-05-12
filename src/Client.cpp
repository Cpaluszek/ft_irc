#include "Client.hpp"

Client::Client(): socketFd(0), isRegistered(false), hasPassword(false) {
}

Client::~Client() {
}

void Client::updateNickname(std::string newNickname) {
	this->previousNickname = this->nickName;
	this->nickName = newNickname;
}

std::ostream &operator<<(std::ostream &out, const Client &src) {
	out << "Client: socket(" << src.socketFd << ") - nickname(" << src.nickName <<") - username(" << src.userName \
		<< ") - isRegistered(" << src.isRegistered <<") - hasPassword(" << src.hasPassword <<")" <<std::endl;
	return out;
}
