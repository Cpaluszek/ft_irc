#pragma once
#include "Server.hpp"

#define PRIVMSG_FORMAT(nick, target, user, host, message) (SERVER_NAME + " " + nick + "!" + user + "@" + host + " PRIVMSG " + nick + " :" + message + "\r\n")

#define ERR_MSG(content) ("Error :" + content + "\r\n")

// 001 - RPL_WELCOME
#define RPL_WELCOME(nick, user ,host) ( ":" + SERVER_NAME + " 001 " + nick \
		+ " :Welcome to the " + SERVER_NAME + " Network, " + nick + "!" + user + "@" + host + "\r\n")

// 002
#define RPL_YOURHOST(nick) (":" + SERVER_NAME + " 002 " + nick \
		+ " :Your host is " + SERVER_NAME + ", running version " + VERSION + "\r\n")

// 003
#define RPL_CREATED(nick, creationDate) (":" + SERVER_NAME + " 003 " + nick \
		+ ":This server was created " + creationDate + "\r\n")

// 004
#define RPL_MYINFO(nick) (":" + SERVER_NAME + " 004 " + nick \
		+ " " + SERVER_NAME + " " + VERSION + " " + USERMODE + " " + CHANMODE + "\r\n")

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

// 401
# define ERR_NOSUCHNICK(nick, target) (":" + SERVER_NAME + " 401 " + nick + " '" + target + "'" \
		+ " :No such nick/channel:\r\n")

//411
# define ERR_NORECIPIENT(nick, command) (":" + SERVER_NAME + " 411 " + \
		nick + " :No recipient given (command)\r\n")

//412
# define ERR_NOTEXTTOSEND(nick) (":" + SERVER_NAME + " 412 " + nick + " :No text to send\r\n")

//403
# define ERR_NOSUCHCHANNEL(nick, channel) (":" + SERVER_NAME + " 403 " + nick + " " + channel + " :No such channel\r\n")

//404
# define ERR_CANNOTSENDTOCHAN(nick, channel) (":" + SERVER_NAME + " 404 " + nick + " " + channel + " :Cannot send to channel\r\n")

//301
# define RPL_AWAY(nick, target, awayMsg) (":" + SERVER_NAME + " 301 " + nick + " " + target + " :awayMsg\r\n")

// 407
# define ERR_TOOMANYTARGETS	(":" + SERVER_NAME + " 407 " + "Too many target for private message\r\n")