/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:47:26 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/12 13:07:11 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

class Server;
class Client;
class Request;

// PASS
void passCmd(Client *client, const Request &request, Server *server);

// NICK
void nickCmd(Client *client, const Request &request, Server *server);

// USER
void userCmd(Client *client, const Request &request, Server *server);

// QUIT
void quitCmd(Client *client, const Request &request, Server *server);

// PRIVMSG
void privmsgCmd(Client *client, const Request &request, Server *server);

// JOIN
void joinCmd(Client *client, const Request &request, Server *server);

// WHOIS
void whoisCmd(Client *client, const Request &request, Server *server);

// WHO
void whoCmd(Client *client, const Request &request, Server *server);

// LIST
void listCmd(Client *client, const Request &request, Server *server);

// MOTD
void motdCmd(Client *client, const Request &request, Server *server);

// PING
void pingCmd(Client *client, const Request &request, Server *server);

// PART
void partCmd(Client *client, const Request &request, Server *server);

// TOPIC
void	topicCmd( Client *client, const Request &request, Server *server);
// NAMES
void namesCmd(Client *client, const Request &request, Server *server);

// AWAY
void awayCmd(Client *client, const Request &request, Server *server);

// INVITE
void inviteCmd(Client *client, const Request &request, Server *server);

// KICK
void    kickCmd(Client *client, const Request &request, Server *server);

// MODE
void mode( Client *client, const Request &request, Server *server );
