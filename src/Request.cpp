
#include "Request.hpp"

Request::~Request() {}

// Todo: code a proper request parsing
Request::Request(std::string rawString) : isValid(true) {
	if (rawString.length() == 0 || rawString[0] == ' ') {
		this->isValid = false;
		return ;
	}
#ifdef DEBUG
	std::cout << YELLOW << "# \"" << rawString << "\" ---> command:";
#endif
	size_t pos;
	while ((pos = rawString.find(' ')) != std::string::npos) {
		this->args.push_back(rawString.substr(0, pos));
		rawString.erase(0, pos + 1);
	}
	this->args.push_back(rawString);
	this->command = this->args[0];
	this->args.erase(this->args.begin());
#ifdef DEBUG
	std::cout << this->command << " - args {";
	for (std::vector<std::string>::iterator it = this->args.begin(); it < this->args.end(); it++) {
		std::cout << *it << ".";
	}
	std::cout << "}" << RESET << std::endl;
#endif
}
