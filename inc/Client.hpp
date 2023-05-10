#pragma once

#include <iostream>

// Note: should we use privates vars + getters and setters
class Client {
public:
	Client();
	~Client();

	int 		socketFd;
	bool		isRegistered;
	std::string nickName;
	std::string userName;
};
