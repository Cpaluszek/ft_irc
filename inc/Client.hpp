#pragma once

#include <iostream>

class Client {
public:
	Client();
	~Client();

private:
	int _socketFd;
	std::string _nickName;
	std::string _userName;
};
