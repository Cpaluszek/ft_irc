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
	bool		hasPassword;
	bool		isAway;
	std::string nickName;
	std::string userName;
	std::string realName;
	std::string previousNickname;
	std::string socketBuffer;
	std::string awayMessage;

	Client();
	~Client();
	void		updateNickname(const std::string& newNickname);
	void		addChannel(Channel *newChannel);
	void 		eraseChannel(const std::string& channel);
	size_t 		getNumberOfChannelsJoined() const;
	channelMap	getChannels() const;
	bool 		isOnChannel(std::string channel) const;

	void		quit();
	std::string getMode() const;
	bool		hasMode(char c) const;
	void		addMode(char c);
	void		removeMode(char c);

private:
	channelMap	_channels;
	std::string _mode;
};

std::ostream &operator<<(std::ostream &out, const Client &src);