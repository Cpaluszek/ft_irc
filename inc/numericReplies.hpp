#pragma once
#include "Server.hpp"

#define ERR_MSG(content) ("Error :" + content + "\r\n")

#define RPL_CMD(nick, user, cmd, reply) (":" + nick + "!" + user + "@" + LOCAL_HOST_IP + " " + cmd \
		+ " " + reply + "\r\n")

#define RPL_INVITE(nickname, user, target, channel) (":" + nickname + "!" + user + "@" + LOCAL_HOST_IP \
	+ " INVITE " + target + " " + channel + "\r\n")

#define RPL_NICK(oldNick, nick, user) ( ":" + oldNick + "!"  \
	+ user + "@" + LOCAL_HOST_IP + " NICK " + nick + "\r\n")

#define RPL_UPDATE_USER_CHAN_MODE(nick, user, channel, sign, mode ,arg) (":" + nick + "!" + user + "@" + LOCAL_HOST_IP \
	+ " MODE " + channel + " " + sign + mode + " " + arg + "\r\n")


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

// 005
#define RPL_ISUPPORT(nick, tokens) (":" + SERVER_NAME + " 005 " + nick \
		+ " " + tokens + " :are supported by this server\r\n")

// 301
#define RPL_AWAY(nick, target, message) (":" + SERVER_NAME + " 301 " + nick \
		+ " " + target + " :" + message + "\r\n")

// 305
#define RPL_UNAWAY(nick) (":" + SERVER_NAME + " 305 " + nick \
		+ " :You are no longer marked as being away\r\n")

// 306
#define RPL_NOWAWAY(nick) (":" + SERVER_NAME + " 306 " + nick \
		+ " :You are marked as being away\r\n")

// 307
#define RPL_WHOISREGNICK(client, nick) (":" + SERVER_NAME + " 307 " + client + " " \
		+ nick + " :has identified for this nick\r\n")

// 311
#define RPL_WHOISUSER(client, nick, user, realname) (":" + SERVER_NAME + " 311 " + client \
		+ " " + nick + " " + user + " " + LOCAL_HOST_IP + " * :" + realname + "\r\n")

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

// 321
#define RPL_LISTSTART(client) (":" + SERVER_NAME + " 321 " + client \
		+ " Channel: Users Name\r\n")

// 322
#define RPL_LIST(client, channel, count, topic) (":" + SERVER_NAME + " 322 " + client \
		+ " " + channel + " " + count + " :" + topic + "\r\n")

// 323
#define RPL_LISTEND(client) (":" + SERVER_NAME + " 323 " + client \
		+ " :End of /LIST\r\n")

// 324
#define RPL_CHANNELMODEIS( nick, channel, modestring ) (":" + SERVER_NAME + " 324 " + nick + \
		+ " " + channel + " " + modestring + "\r\n")

// 329
#define RPL_CREATIONTIME( nick, channel, creationTime ) (":" + SERVER_NAME + " 329 " + nick + \
		+ " " + channel + " " + creationTime + "\r\n")

// 331 : topic is not set yet on channel
#define RPL_NOTOPIC(nick, channel, topic) (":" + SERVER_NAME + " 331 " + nick + \
		+ " " + channel + " :No topic is set\r\n")

// 332 : Sent to a client when joining the channel to inform of the current topic
#define RPL_TOPIC(nick, channel, topic) (":" + SERVER_NAME + " 332 " + nick + \
		+ " " + channel + " :" + topic + "\r\n")

// 333
#define RPL_TOPICWHOTIME(nick, channel, who, time) (":" + SERVER_NAME + " 333 " + nick + \
		+ " " + channel + " " + who + " " + time + "\r\n")

// 341
#define RPL_INVITING(client, nick, channel) (":" + SERVER_NAME + " 341 " + client \
		+ " " + nick + " " + channel + "\r\n")

// 352
#define RPL_WHOREPLY(client, channel, user, nick, flag, realname) (":" + SERVER_NAME + " 352 " + client \
		+ " " + channel + " " + user + " " + LOCAL_HOST_IP + " " + SERVER_NAME + " " + nick + " " + flag \
		+ " :0 " + realname + "\r\n")

// 353
#define RPL_NAMREPLY(nick, symbol, channel, names) (":" + SERVER_NAME + " 353 " + nick + \
		+ " " + symbol + " " + channel + " " + names + "\r\n")

// 366
#define RPL_ENDOFNAMES(nick, channel) (":" + SERVER_NAME + " 366 " + nick + \
		+ " " + channel + " :End of NAMES list\r\n")

// 372
#define RPL_MOTD(nick, line) (":" + SERVER_NAME + " 372 " + nick + \
		+ " :" + line + "\r\n")

// 375
#define RPL_MOTDSTART(nick) (":" + SERVER_NAME + " 375 " + nick + \
		+ " :- " + SERVER_NAME + " Message of the day - \r\n")

// 376
#define RPL_ENDOFMOTD(nick) (":" + SERVER_NAME + " 376 " + nick + \
		+ " :End of MOTD.\r\n")

// 378
#define RPL_WHOISHOST(client, nick) (":" + SERVER_NAME + " 378 " + client \
		+ " " + nick + " :is connecting from *@localhost 127.0.0.1\r\n")

// 379
#define RPL_WHOISMODES(client, nick, modes) (":" + SERVER_NAME + " 379 " + client \
		+ " " + nick + " :is using modes " + modes + "\r\n")

// 401
# define ERR_NOSUCHNICK(nick, target) (":" + SERVER_NAME + " 401 " + nick + " '" + target + "'" \
		+ " :No such nick/channel:\r\n")

// 403
#define ERR_NOSUCHCHANNEL(nick, channel) (":" + SERVER_NAME + " 403 " + nick \
		+ " " + channel + " :No such channel\r\n")

// 404
# define ERR_CANNOTSENDTOCHAN(nick, channel) (":" + SERVER_NAME + " 404 " + nick + " " + channel + " :Cannot send to channel\r\n")

// 405
#define ERR_TOOMANYCHANNELS(nick, channel) (":" + SERVER_NAME + " 405 " + nick \
		+ " " + channel + " :You have joined too many channel\r\n")

// 407
# define ERR_TOOMANYTARGETS	(":" + SERVER_NAME + " 407 " + ":Too many target for private message\r\n")

// 411
# define ERR_NORECIPIENT(nick, command) (":" + SERVER_NAME + " 411 " + \
		nick + " :No recipient given (command)\r\n")

// 412
# define ERR_NOTEXTTOSEND(nick) (":" + SERVER_NAME + " 412 " + nick + " :No text to send\r\n")

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

// 441
# define ERR_USERNOTINCHANNEL(client, nick, channel) (":" + SERVER_NAME + " 441 " + client + " " + nick + " " + channel + " :They're not on that channel\r\n")

// 442
# define ERR_NOTONCHANNEL(nick, channel) (":" + SERVER_NAME + " 442 " + nick + " " + channel + " :You're not on that channel\r\n")

// 443
#define ERR_USERONCHANNEL(client, nick, channel) (":" + SERVER_NAME + " 443 " + client \
		+ " " + nick + " " + channel + " :is already on channel\r\n")

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
#define ERR_CHANNELISFULL(nick, channel) (":" + SERVER_NAME + " 471 " + nick \
		+ " " + channel + " :Cannot join channel (+l)\r\n")

// 473
#define ERR_INVITEONLYCHAN(nick, channel) (":" + SERVER_NAME + " 473 " + nick \
		+ " " + channel + " :Cannot join channel (+i)\r\n")

// 474
#define ERR_BANNEDFROMCHAN

// 475
#define ERR_BADCHANNELKEY(nick, channel) (":" + SERVER_NAME + " 475 " + nick \
		+ " " + channel + " :Cannot join channel (+k)\r\n")

// 476
#define ERR_BADCHANMASK(channel) (":" + SERVER_NAME + " 476 " + channel + " :Bad Channel Mask\r\n")

// 481
#define ERR_NOPRIVILEGES( nick ) (":" + SERVER_NAME + " 481 " + nick + " :" + "Permission Denied- \
		You're not an IRC operator/r/n")

// 482
#define ERR_CHANOPRIVSNEEDED(nick, channel) (":" + SERVER_NAME + " 482 " + nick \
		+ " " + channel + " :You're not channel operator\r\n")

// 501
#define ERR_UMODEUNKNOWNFLAG( nick, modes ) (":" + SERVER_NAME + " 501 " + nick + " :Unknown MODE flag : '" + modes + "\r\n")