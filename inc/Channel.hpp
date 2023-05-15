#pragma once

#include <iostream>

#include "Client.hpp"

// [IRC Client Protocol Specification](https://modern.ircdocs.horse/#channels)

// Todo: founder + operator
class Channel {
public:
	std::string name;
	std::string topic;
	std::string mode;


private:
	// Todo: container of all the connected clients

	// Functions:
		// Connect client
		// Disconnect client

};

