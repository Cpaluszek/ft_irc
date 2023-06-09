/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:47:34 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:47:34 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include "colors.hpp"
#include "Utils.hpp"
#include "Client.hpp"

class Client;
class Server;
class Request {
public:
	Request(std::string message);
	Request();
	~Request();

	bool isValid;
	std::string command;
	std::vector<std::string> args;

	bool requestPrivMsgIsValid(Client *client, const Request &request) const;
	bool requestTopicIsValid(Client *client) const;
	bool requestModeIsValid(Client *client, Server *server) const;
};

std::ostream &operator<<(std::ostream &out, const Request &src);
