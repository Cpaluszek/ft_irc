/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:44:07 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:44:07 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Client.hpp"

Request::~Request() {}

Request::Request(std::string message): isValid(true) {
	if (message.length() == 0 || message[0] == ' ') {
		this->isValid = false;
		return ;
	}
	std::vector<std::string> vecStr = Utils::split(message, " ", true);
	if (vecStr.empty()) {
		this->isValid = false;
		return ;
	}

	// Parse commad
	this->command = vecStr[0];
	vecStr.erase(vecStr.begin());
	std::string::iterator it;
	for (it = this->command.begin(); it != this->command.end(); it++) {
		if (!std::isalnum(*it)) {
			this->isValid = false;
			return ;
		}
		*it = toupper(*it);
	}

	// Parse parameters
	std::vector<std::string>::iterator itVec;
	for (itVec = vecStr.begin(); itVec != vecStr.end(); itVec++) {
//		if ((*itVec)[0] == ':') {
//			*itVec = (*itVec).erase(0, 1);
//		}
		this->args.push_back(*itVec);
	}
}

Request::Request() : isValid(true) {}

std::ostream &operator<<(std::ostream &out, const Request &src) {
	out << "\" ---> command:[" << src.command << "] - args {";
	for (std::vector<std::string>::const_iterator it = src.args.begin(); it < src.args.end(); it++) {
		out << *it << ";";
	}
	out << "}" << RESET << std::endl;
	return out;
}
