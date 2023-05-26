#include "Utils.hpp"

std::string Utils::getCurrentDateTime() {
	time_t     now = time(0);
	char       buf[80];

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", localtime(&now));
	return buf;
}

std::string Utils::copyToUpper(std::string src) {
	std::string res = src;
	std::transform(src.begin(), src.end(), src.begin(), toupper);
	return res;
}

std::vector<std::string> Utils::split(const std::string& source, const std::string& delimiter) {
	std::vector<std::string> result;
	std::string subStr;
	size_t pos = -1;
	size_t prevPos = 0;
	while ((pos = source.find_first_of(delimiter, pos + 1)) != std::string::npos) {
		subStr = source.substr(prevPos, pos - prevPos);
		if (!subStr.empty()) {
			result.push_back(subStr);
		}
		prevPos = pos + 1;
		if (source[prevPos] == ':') {
			break ;
		}
	}
	pos = source.find_first_of("\r\n");
	subStr = source.substr(prevPos, pos - prevPos);
	if (!subStr.empty()) {
		result.push_back(subStr);
	}
	return result;
}

size_t 	Utils::getSemicolPos( const std::string &arg )
{
	size_t pos = arg.find( ':' );
	return pos;
}

