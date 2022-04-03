#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstddef>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cerrno>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "Socket.hpp"

#define SERVER_PORT "3490"
#define TRUE 1
#define FALSE 0


class Socket
{
private:
	const char			*port;
	struct addrinfo		hints;
	struct addrinfo		*res;
	int					listen_sock;

public:
	Socket();
	Socket(const char *customPort);
	Socket(const Socket &other);
	Socket				&operator=(const Socket &other);
	~Socket();
	const char 			&getPort() const;
	addrinfo			 *getRes() const;
	int		 			getListenSock() const;
	void 				setPort(const char *other_port);
	void				createAddrinfo();
	void 				createSocket();
};

#endif