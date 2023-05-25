#pragma once

#include <iostream>
#include <map>
#include "Channel.hpp"
#include "commands.hpp"

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
	void 		eraseChannel(std::string channel);
	bool 		isOnChannel(std::string channel) const;
	void		quit();
	std::string getMode() const;

private:
	channelMap	_channels;
	std::string _mode;
};

std::ostream &operator<<(std::ostream &out, const Client &src);