
#pragma once

#include <iostream>
#include <vector>

class Request {
public:
	Request(std::string rawString);
	~Request();

	bool						isValid;
	std::string					command;
	std::vector<std::string> 	args;
};
