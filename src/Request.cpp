
#include "Request.hpp"

Request::~Request() {}

Request::Request(std::string message): isValid(true) {
	if (message.length() == 0 || message[0] == ' ') {
		this->isValid = false;
		return ;
	}
	std::vector<std::string> vecStr = split(message, " ");
	if (vecStr.empty()) {
		this->isValid = false;
		return ;
	}

//	// Todo: check for tags - '@'
//	if (vecStr[0][0] == '@') {
//		this->tags = split(vecStr[0], ";");
//		this->tags[0].erase(0, 1);
//		vecStr.erase(vecStr.begin());
//	}
//
//	// Todo: check for source - ':'
//	if (vecStr[0][0] == ':') {
//		this->source = vecStr[0];
//		this->source.erase(0, 1);
//		vecStr.erase(vecStr.begin());
//	}

	// Parse commad
	this->command = vecStr[0];
	vecStr.erase(vecStr.begin());
	std::string::iterator it;
	for (it = this->command.begin(); it != this->command.end(); it++) {
		if (!std::isalnum(*it)) {
			this->isValid = false;
			return ;
		}
		*it = toupper(*it);
	}

	// Parse parameters
	std::vector<std::string>::iterator itVec;
	for (itVec = vecStr.begin(); itVec != vecStr.end(); itVec++) {
		if ((*itVec)[0] == ':') {
			*itVec = (*itVec).erase(0, 1);
		}
		this->args.push_back(*itVec);
	}
}

std::vector<std::string> Request::split(const std::string& source, const std::string& delimiter) {
	std::vector<std::string> result;
	std::string subStr;
	size_t pos = -1;
	size_t prevPos = 0;
	while ((pos = source.find_first_of(delimiter, pos + 1)) != std::string::npos) {
		subStr = source.substr(prevPos, pos - prevPos);
		if (!subStr.empty()) {
			result.push_back(subStr);
		}
		prevPos = pos + 1;
		if (source[prevPos] == ':') {
			break ;
		}
	}
	pos = source.find_first_of("\r\n");
	subStr = source.substr(prevPos, pos - prevPos);
	if (!subStr.empty()) {
		result.push_back(subStr);
	}
	return result;
}

std::ostream &operator<<(std::ostream &out, const Request &src) {
	out << "\" ---> command:[" << src.command << "] - args {";
	for (std::vector<std::string>::const_iterator it = src.args.begin(); it < src.args.end(); it++) {
		out << *it << "_";
	}
	out << "} - tags {";
	for (std::vector<std::string>::const_iterator it = src.tags.begin(); it < src.tags.end(); it++) {
		out << *it << "_";
	}
	out << "} - source: " << src.source << RESET << std::endl;
	return out;
}
