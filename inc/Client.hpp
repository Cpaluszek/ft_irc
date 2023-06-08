/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:47:20 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:47:20 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	void		leaveAllChannels();

	// ---- Channels ----
	void		addChannel(Channel *newChannel);
	void 		eraseChannel(const std::string& channel);
	size_t 		getNumberOfChannelsJoined() const;
	channelMap	getChannels() const;
	bool 		isOnChannel(const std::string& channel) const;

	// ---- Mode ----
	std::string getMode() const;
	bool		hasMode(char c) const;
	void		addMode(char c);
	void		removeMode(char c);

private:
	channelMap	_channels;
	std::string _mode;
};

std::ostream &operator<<(std::ostream &out, const Client &src);