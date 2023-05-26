#include <iostream>
#include "Server.hpp"

// Todo: do we need to manage signals ? SIQINT - SIQQUIT - SIGKILL ?

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	try {
		Server serv(argv[1], argv[2]);
		while (true) {
			serv.Update();
		}
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
