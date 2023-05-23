#pragma once

#include <iostream>
#include <map>
#include "Channel.hpp"

class Channel;

// Note: should we use privates vars + getters and setters
class Client {
public:
	typedef std::map<std::string, Channel*> channelMap;
	typedef std::map<std::string, Channel*>::iterator channelMapIt;

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
	void		updateNickname(const std::string& newNickname);
	void		addChannel(Channel *newChannel);
	// Todo: remove channel
	size_t 		getNumberOfChannelsJoined() const;
	channelMap	getChannels() const;
	bool 		isOnChannel(std::string channel) const;
	std::string getMode() const;

private:
	channelMap _channels;
	std::string _mode;
};

std::ostream &operator<<(std::ostream &out, const Client &src);