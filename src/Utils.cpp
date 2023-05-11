#include "Utils.hpp"

std::string Utils::getCurrentDateTime() {
	time_t     now = time(0);
	char       buf[80];

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", localtime(&now));
	return buf;
}
