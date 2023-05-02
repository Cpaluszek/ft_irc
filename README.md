# ft_irc
This project is about creating our own IRC server.
We will use an actual IRC client to connect to the server and test it.

## IRC 
**Internet Relay Chat** is a text-based communication protocol on the Internet.
It offers real-time messaging that can either be public or private.
Users can exchange direct messages and join group channels.

IRC clients connect to IRC servers in order to join channels. IRC servers are connected together to form a network.

## Mandatory Part
Develop an IRC server in C++ 98.

Executable will be run as follows:
`./ircserv <port> <password>`

### Requirements
- The server must be capable of handling multiple clients at the same time and never hang.
- Forking is not allowed. All I/O operations must be *non-blocking*.
- Only 1 `poll()` or equivalent can be used for handling all these operations (read, write, listen).
- Several IRC clients exists. Choose one of them as a *reference*.
- Communication between client and server has to be done via TCP/IP (v4 or v6)
- Implement the following features:
	- Authenticate, set a nickname, a username, join a channel, send and receive private messages.
	- All the messages sent from one client to a channel have to be forwarded to every other client on the channel.
	- Have *operators* and regular users.
	- Implement commands that are specific to operators.

## Documentation
### Socket Programming
### Socket creation
```cpp
int sockfd = socket(domain, type, protocol);
```

- *sockfd*: socket descriptor (similar to file descriptor)
- *domain:* integer, specifies communication domain. 
	- *AF_LOCAL*: communication between processes on the same host
	- *AF_INET* - *AF_INET6*: communication between different hosts (IPV4 - 6)
- *type*: communication type
	- *SOCK_STREAM*: TCP
	- *SOCKER_DGRAM*: UDP
- *protocol*: protocol value for Internet Protocol (IP), which is 0
	- Same number which appears on protocol field in the IP header of a packer (man protocols)

### Socket options
```cpp
int setsockopt(int sockfd, int level, int optname,  const void *optval, socklen_t optlen);
```
This helps set the options for the socket referred by the file descriptor.
Completely optional, but it helps in reuse of address and port.
- Prevents error such as: "address already in use"

### Bind
```cpp
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

After the creation of the socket, this function binds the socket to the address and port number specified.

### Listen
```cpp
int listen(int sockfd, int backlog);
```

Puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection.
- The backlog, defines the maximum length to which the queue of pending connections for sockdf may grow.

### Accept
```cpp
int new_socket = accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

Extracts the first connection request on the queue of pending connections and creates a new connected socket and return a new file descriptor.
**At this point the connection is established between client and server, and they are ready to transfer data.**


## References
- IRC client used: `hexchat`

### IRC
- [Internet Relay Chat - Wikipedia](https://en.wikipedia.org/wiki/Internet_Relay_Chat)
- [RFC 1459: Internet Relay Chat Protocol](https://www.rfc-editor.org/rfc/rfc1459)

### IRC client
- HexChat: [Client Documentation — HexChat 2.16.0 documentation](https://hexchat.readthedocs.io/en/latest/)

### Socket Programming
- [Socket Programming in C/C++ - GeeksforGeeks](https://www.geeksforgeeks.org/socket-programming-cc/)
- [Socket Programming in C/C++: Handling multiple clients on server without multi threading - GeeksforGeeks](https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/)

