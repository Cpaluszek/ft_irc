#pragma once

#include "Server.hpp"

class Server;

// PASS
void passCmd(Client client, const Request &request, Server *server);

// NICK
void nickCmd(Client client, const Request &request, Server *server);

// USER
void userCmd(Client client, const Request &request, Server *server);

