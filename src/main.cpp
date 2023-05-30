#include <iostream>
#include "Server.hpp"

// Todo: manage signals SIQINT - SIQQUIT - SIGKILL ? To avoid leaks in case of interrupt

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
