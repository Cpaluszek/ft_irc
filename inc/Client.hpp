#pragma once

#include <iostream>

// Note: should we use privates vars + getters and setters
class Client {
public:
	int 		socketFd;
	// If any command is executed and client is not registered -> send 462
	bool		isRegistered;
	bool		hasPassword;
	std::string nickName;
	std::string userName;
	std::string realName;
	std::string previousNickname;
	std::string socketBuffer;


	Client();
	~Client();
	void updateNickname(const std::string& newNickname);
};

std::ostream &operator<<(std::ostream &out, const Client &src);