#include <iostream>
#include "../inc/Server.hpp"

// Todo: remove socket_example directory + makefile rule

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	try {
		Server serv(argv[1], argv[2]);
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
