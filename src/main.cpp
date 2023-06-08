/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:44:04 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:44:04 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	signal(SIGINT, Server::handleKeyboardInterrupt);
	try {
		Server serv(argv[1], argv[2]);
		serv.Update();
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
