#pragma once

#include "Server.hpp"

class Server;

void passCmd(Client client, const Request &request, Server *server);
