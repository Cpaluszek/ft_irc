
#pragma once

#include <iostream>
#include <vector>
#include "colors.hpp"
#include "Utils.hpp"

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
};

std::ostream &operator<<(std::ostream &out, const Request &src);
