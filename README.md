# ft_irc

This project is about creating our own IRC server.
We will use an actual IRC client to connect to the server and test it.
Internet is ruled by solid standards protocols that allow connected computers to interact with each other.

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

## References
- IRC client used: `hexchat`

### IRC
- [Internet Relay Chat - Wikipedia](https://en.wikipedia.org/wiki/Internet_Relay_Chat)
- [RFC 1459: Internet Relay Chat Protocol](https://www.rfc-editor.org/rfc/rfc1459)

