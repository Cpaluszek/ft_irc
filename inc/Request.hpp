
#pragma once

#include <iostream>
#include <vector>

#define DEBUG_REQUEST 1

class Request {
public:
	Request(std::string rawString);
	~Request();

	bool						isValid;
	std::string					command;
	std::vector<std::string> 	args;
};
