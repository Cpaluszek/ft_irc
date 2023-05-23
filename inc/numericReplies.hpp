#pragma once
#include "Server.hpp"

#define ERR_MSG(content) ("Error :" + content + "\r\n")

#define RPL_CMD(nick, user, cmd, reply) (":" + nick + "!" + user + "@" + SERVER_IP + " " + cmd \
		+ " " + reply + "\r\n")

// 001 - RPL_WELCOME
#define RPL_WELCOME(nick, user) ( ":" + SERVER_NAME + " 001 " + nick \
		+ " :Welcome to the " + SERVER_NAME + " Network, " + nick + "!" + user + "@" + SERVER_IP + "\r\n")

// 002
#define RPL_YOURHOST(nick) (":" + SERVER_NAME + " 002 " + nick \
		+ " :Your host is " + SERVER_NAME + ", running version " + VERSION + "\r\n")

// 003
#define RPL_CREATED(nick, creationDate) (":" + SERVER_NAME + " 003 " + nick \
		+ " :This server was created " + creationDate + "\r\n")

// 004
#define RPL_MYINFO(nick) (":" + SERVER_NAME + " 004 " + nick \
		+ " " + SERVER_NAME + " " + VERSION + " " + USERMODE + " " + CHANMODE + "\r\n")

// 307
#define RPL_WHOISREGNICK(client, nick) (":" + SERVER_NAME + " 307 " + client + " " \
		+ nick + " :has identified for this nick\r\n")

// 311
#define RPL_WHOISUSER(client, nick, user, realname) (":" + SERVER_NAME + " 311 " + client \
		+ " " + nick + " " + user + " " + SERVER_IP + " * :" + realname + "\r\n")

// 312
#define RPL_WHOISSERVER(client, nick, serverInfo) (":" + SERVER_NAME + " 312 " + client \
		+ " " + nick + " " + SERVER_NAME + " :" + serverInfo + "\r\n")

// 313
#define RPL_WHOISOPERATOR(client, nick) (":" + SERVER_NAME + " 313 " + client \
		+ " " + nick + " :is an IRC operator\r\n")

// 315
#define RPL_ENDOFWHO(nick, maks) (":" + SERVER_NAME + " 315 " + nick \
		+ " " + mask + " :End of WHO list\r\n")

// 318
#define RPL_ENDOFWHOIS(client, nick) (":" + SERVER_NAME + " 318 " + client \
		+ " " + nick + " :End of WHOIS list\r\n")

// 319
#define RPL_WHOISCHANNEL(client, nick, list) (":" + SERVER_NAME + " 319 " + client \
		+ " " + nick + " :" + list + "\r\n")

// 332
#define RPL_TOPIC(nick, channel, topic) (":" + SERVER_NAME + " 332 " + nick + \
		+ " " + channel + " :" + topic + "\r\n")

// 333
#define RPL_TOPICWHOTIME(nick, channel, who, time) (":" + SERVER_NAME + " 333 " + nick + \
		+ " " + channel + " " + who + " " + time + "\r\n")

// 352
#define RPL_WHOREPLY(client, channel, user, nick, flag, realname) (":" + SERVER_NAME + " 352 " + client \
		+ " " + channel + " " + user + " " + SERVER_IP + " " + SERVER_NAME + " " + nick + " " + flag \
		+ " :0 " + realname + "\r\n")

// 353
#define RPL_NAMREPLY(nick, symbol, channel, names) (":" + SERVER_NAME + " 353 " + nick + \
		+ " " + symbol + " " + channel + " " + names + "\r\n")

// 366
#define RPL_ENDOFNAMES(nick, channel) (":" + SERVER_NAME + " 366 " + nick + \
		+ " " + channel + " :End of /NAMES list\r\n")

// 372
#define RPL_MOTD(nick, line) (":" + SERVER_NAME + " 372 " + nick + \
		+ " :" + line + "\r\n")

// 375
#define RPL_MOTDSTART(nick) (":" + SERVER_NAME + " 375 " + nick + \
		+ " :- " + SERVER_NAME + " Message of the day - \r\n")

// 376
#define RPL_ENDOFMOTD(nick) (":" + SERVER_NAME + " 376 " + nick + \
		+ " :End of /MOTD command.\r\n")

// 378
#define RPL_WHOISHOST(client, nick) (":" + SERVER_NAME + " 378 " + client \
		+ " " + nick + " :is connecting from *@localhost 127.0.0.1\r\n")

// 379
#define RPL_WHOISMODES(client, nick, modes) (":" + SERVER_NAME + " 379 " + client \
		+ " " + nick + " :is using modes " + modes + "\r\n")

// 403
#define ERR_NOSUCHCHANNEL(nick, channel) (":" + SERVER_NAME + " 403 " + nick \
		+ " " + channel + " :No such channel\r\n")

// 405
#define ERR_TOOMANYCHANNELS(nick, channel) (":" + SERVER_NAME + " 405 " + nick \
		+ " " + channel + " :You have joined too many channel\r\n")

// 421
#define ERR_UNKNOWCOMMAND(nick, command) (":" + SERVER_NAME + " 421 " + nick \
		+ " " + command + " :Unknown command\r\n")

// 422
#define ERR_NOMOTD(nick) (":" + SERVER_NAME + " 422 " + nick + \
		+ " :MOTD File is missing\r\n")

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