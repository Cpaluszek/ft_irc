#pragma once

// Numeric replies list: C for code - M for message

#define NO_RESPONSE ""

#define ERR_MSG(content) ("Error: " + content + "\r\n")

// 001 - RPL_WELCOME
#define RPL_WELCOME(nick, user ,host) ( ":" + SERVER_NAME + " 001 " + nick \
		+ " :Welcome to the " + SERVER_NAME + " Network, " + nick + "!" + user + "@" + host + "\r\n")

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
