#include <cstdio>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 3022

int main(void) {
	int serverFd, newSocket;
	struct sockaddr_in address;
	int opt = 1;
	int addrLen = sizeof(address);
	char buffer[1024] = { 0 };
	std::string helloStr = "Hello from server";

	// Creating socket file descriptor
	//		AF_INET		= address family IPV4
	//		SOCK_STREAM	= TCP protocol
	//		0 			= default protocol for the given address family
	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0) {
		std::perror("socket failed");
		return EXIT_FAILURE;
	}

	// Set socket options
	//		SOL_SOCKET	= level at which the option is defined, in this case the socket level
	//		SO_REUSADDR	= allow the socket to reuse a local IP address and port number that is still in the TIME_WAIT state after a previous communication
	//		opt			= Non-zero to enable a boolean option
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		std::perror("setsockopt failed");
		return EXIT_FAILURE;
	}

	address.sin_family = AF_INET;				// Set the address familly to IPv4
	address.sin_addr.s_addr = INADDR_ANY;		// Means the socket can receive packets from any interface
	address.sin_port = htons(PORT);	// Set the port number
	// htons() is used to convert a 16-bit number in host byte order to a network byte order

	// bind(): associate the socket with an IP address and a port number so that it can receive incoming network packets
	// bind() is called on the server side - connect() on the client side
	if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		std::perror("bind failed");
		return EXIT_FAILURE;
	}

	// Listen for incoming connections on the socket, sets up a queue for incoming connections and return immediately
	// The second argument is the maximum number of connections that can be queued up (referred as backlog)
	if (listen(serverFd, 3) < 0) {
		std::perror("listen failed");
		return EXIT_FAILURE;
	}

	// After calling listen() the server can call accept() to accept incoming connections on the socket.
	// Extract the first connection request on the queue of pending connections
	// Creates a new connected socket, and return a new file descriptor
	newSocket = accept(serverFd, (struct sockaddr*)&address, (socklen_t*)&addrLen);
	if (newSocket < 0) {
		std::perror("accept failed");
		return EXIT_FAILURE;
	}
	// At this point the connection is established between client and server, and they are ready to transfer data

	// recv() is used to read incoming data on sockets
	// Will return as much data as is currently available, up to the size of the buffer specified
	// when receiving large data with recv(), it's necessary to use a smaller fixed-length buffer when reading from the socket,
	// and then append the received data to a dynamically growing buffer (like a std::string, or a file) on each loop iteration.
	// recv() tells you how many bytes were actually received, so do not access more than that many bytes when accessing the buffer.
	recv(newSocket, buffer, 1024, 0);
	std::cout << "Read: " << buffer << std::endl;

	// Send data through a socket
	// Returns the number of bytes sent, send() may not send all the bytes in the buffer in a single call, this function must be called again
	send(newSocket, helloStr.c_str(), helloStr.length(), 0);
	std::cout << "Hello message sent" << std::endl;

	close(newSocket);
	return 0;
}