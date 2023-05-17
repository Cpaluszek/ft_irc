#pragma once

#include <iostream>
#include <map>
#include "Channel.hpp"

class Channel;

// Note: should we use privates vars + getters and setters
class Client {
public:
	int 		socketFd;
	bool		isRegistered;
	bool		hasPassword;
	std::string nickName;
	std::string userName;
	std::string realName;
	std::string previousNickname;
	std::string socketBuffer;


	Client();
	~Client();
	void	updateNickname(const std::string& newNickname);
	void	addChannel(Channel *newChannel);
	// Todo: remove channel
	size_t getNumberOfChannelsJoined() const;

private:
	std::map<std::string, Channel*> _channels;
};

std::ostream &operator<<(std::ostream &out, const Client &src);