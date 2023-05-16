#pragma once
#include "Server.hpp"

#define ERR_MSG(content) ("Error :" + content + "\r\n")

#define RPL_CMD(nick, user, cmd, reply) (":" + nick + "!" + user + "@" + LOCAL_HOST_IP + " " + cmd \
		+ " " + reply + "\r\n")

// 001 - RPL_WELCOME
#define RPL_WELCOME(nick, user) ( ":" + SERVER_NAME + " 001 " + nick \
		+ " :Welcome to the " + SERVER_NAME + " Network, " + nick + "!" + user + "@" + LOCAL_HOST_IP + "\r\n")

// 002
#define RPL_YOURHOST(nick) (":" + SERVER_NAME + " 002 " + nick \
		+ " :Your host is " + SERVER_NAME + ", running version " + VERSION + "\r\n")

// 003
#define RPL_CREATED(nick, creationDate) (":" + SERVER_NAME + " 003 " + nick \
		+ ":This server was created " + creationDate + "\r\n")

// 004
#define RPL_MYINFO(nick) (":" + SERVER_NAME + " 004 " + nick \
		+ " " + SERVER_NAME + " " + VERSION + " " + USERMODE + " " + CHANMODE + "\r\n")

// 353
#define RPL_NAMREPLY(nick, symbol, channel, names) (":" + SERVER_NAME + " 353 " + nick + \
		+ " " + symbol + " " + channel + " " + names + "\r\n")

// 366
#define RPL_ENDOFNAMES(nick, channel) (":" + SERVER_NAME + " 366 " + nick + \
		+ " " + channel + " :End of /NAMES list\r\n")

// 403
#define ERR_NOSUCHCHANNEL(nick, channel) (":" + SERVER_NAME + " 403 " + nick \
		+ " " + channel + " :No such channel\r\n")

// 405
#define ERR_TOOMANYCHANNELS


// 421
#define ERR_UNKNOWCOMMAND(nick, command) (":" + SERVER_NAME + " 421 " + nick \
		+ " " + command + " :Unknown command\r\n")

// 431
#define ERR_NONICKNAMEGIVEN(client) (":" + SERVER_NAME + " 431 " + client \
		+ " :No nickname given\r\n")

// 432
#define ERR_ERRONEUSNICKNAME(nick) (":" + SERVER_NAME + " 432 " + nick \
		+ " :Erroneus nickname\r\n")

// 433
#define ERR_NICKNAMEINUSE(nick) (":" + SERVER_NAME + " 433 " + nick \
		+ " :Nickname is already in use\r\n")

#define RPL_NICK(oldNick, nick, user, host) ( ":" + oldNick + "!"  \
	+ user + "@" + host + " NICK " + nick + "\r\n")

// 451
#define ERR_NOTREGISTERED(nick) (":" + SERVER_NAME + " 451 " + nick \
		+  " :You have not registered\r\n")

// 461
#define ERR_NEEDMOREPARAMS(nick, command) (":" + SERVER_NAME + " 461 " + nick + " " \
		+ command + " :Not enough parameters\r\n")

// 462
#define ERR_ALREADYREGISTERED(nick) (":" + SERVER_NAME + " 462 " + nick \
		+  " :You may not reregister\r\n")

// 464
#define ERR_PASSWDMISMATCH(nick) (":" + SERVER_NAME + " 464 " + nick \
		+  " :Password incorrect\r\n")

// 471
#define ERR_CHANNELISFULL

// 473
#define ERR_INVITEONLYCHAN

// 474
#define ERR_BANNEDFROMCHAN

// 475
#define ERR_BADCHANNELKEY(nick, channel) (":" + SERVER_NAME + " 475 " + nick \
		+ " " + channel + " :Cannot join channel (+k)\r\n")

// 476
#define ERR_BADCHANMASK(channel) (":" + SERVER_NAME + " 476 " + channel + " :Bad Channel Mask\r\n")