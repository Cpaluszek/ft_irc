#pragma once

#include "Server.hpp"

class Server;

std::string passCmd(Client client, const Request &request, Server *server);
