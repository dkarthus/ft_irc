#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cerrno>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "Socket.hpp"

#define SERVER_PORT "3490"
#define TRUE 1
#define FALSE 0
//#include "Server.hpp"

class Socket
{
private:
	const char			*port;
	struct addrinfo		hints;
	struct addrinfo		*res;
	int					listen_sock;
public:
public:

	Socket();
	Socket(const Socket &other);
	Socket				&operator=(const Socket &other);
	~Socket();
	const char 			&getPort() const;
	addrinfo			 *getRes() const;
	int		 			getListenSock();
	void 				setPort(const char *other_port);
	void				createAddrinfo();
	void 				createSocket();
};

#endif