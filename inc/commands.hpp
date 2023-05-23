#pragma once

#include "Server.hpp"

class Server;

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

// WHO
void whoCmd(Client *client, const Request &request, Server *server);
