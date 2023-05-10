
#pragma once

#include <iostream>
#include <vector>

#ifndef DEBUG_REQUEST
# define DEBUG_REQUEST
#endif

class Request {
public:
	Request(std::string rawString);
	~Request();

	bool						isValid;
	std::string					command;
	std::vector<std::string> 	args;
};
