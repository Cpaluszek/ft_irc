#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

class Utils {
public:
	static std::string getCurrentDateTime();

	static std::string copyToUpper(std::string src);

	static std::vector<std::string> split(const std::string &source, const std::string &delimiter, bool stopOnSemicolon);

	static size_t 	getSemicolonPos(const std::string &arg );
};
