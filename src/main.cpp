#include <cstdio>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 3022

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;

	int serverFd, newSocket;
	struct sockaddr_in address;
	int opt = 1;
	int addrLen = sizeof(address);
	char buffer[1024] = { 0 };
	std::string helloStr = "Hello from server";

	// Creating socket file descriptor
	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0) {
		std::perror("socket failed");
		return EXIT_FAILURE;
	}

	// Attaching socket to port X
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		std::perror("setsockopt failed");
		return EXIT_FAILURE;
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		std::perror("bind failed");
		return EXIT_FAILURE;
	}

	if (listen(serverFd, 3) < 0) {
		std::perror("listen failed");
		return EXIT_FAILURE;
	}

	newSocket = accept(serverFd, (struct sockaddr*)&address, (socklen_t*)&addrLen);
	if (newSocket < 0) {
		std::perror("accept failed");
		return EXIT_FAILURE;
	}

	read(newSocket, buffer, 1024);
	std::cout << "Read: " << buffer << std::endl;
	send(newSocket, helloStr.c_str(), helloStr.length(), 0);
	std::cout << "Hello message sent" << std::endl;

	close(newSocket);
	shutdown(serverFd, SHUT_RDWR);
	return 0;
}