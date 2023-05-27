
#pragma once

#include <iostream>
#include <vector>
#include "colors.hpp"
#include "Utils.hpp"
#include "Client.hpp"

class Client;
class Request {
public:
	Request(std::string message);
	~Request();

	bool						isValid;
	std::string					command;
	std::vector<std::string> 	args;

	// Note: remove tags and source ?
	std::vector<std::string> 	tags;
	std::string 				source;

	bool req
	bool				requestPrivMsgIsValid( Client *client ) const;
	bool				requestTopicIsValid( Client *client ) const;

};

std::ostream &operator<<(std::ostream &out, const Request &src);
