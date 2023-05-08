#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3022

int main(void) {
	int status, clientFd;
	struct sockaddr_in servAddr;
	std::string helloStr = "Hello from client";
	char buffer[1024] = { 0 };

	clientFd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientFd < 0) {
		std::cout << "Socket creation error" << std::endl;
		return 1;
	}

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0) {
		std::cout << "Invalid address / Address not supported" << std::endl;
		return 1;
	}

	status = connect(clientFd, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (status < 0) {
		std::cout << "Connection failed" << std::endl;
		return 1;
	}
	send(clientFd, helloStr.c_str(), helloStr.length(), 0);
	std::cout << "Hello message sent" << std::endl;
	read(clientFd, buffer, 1024);
	std::cout << "Read: " << buffer << std::endl;
	while (1) {
		std::string input;
		std::cout << "Message: ";
		std::cin >> input;
		if (std::cin.eof()) {
			break ;
		}
		send(clientFd, input.c_str(), input.length(), 0);
	}
	close(clientFd);
	return 0;
}