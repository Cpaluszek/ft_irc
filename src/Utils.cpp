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
