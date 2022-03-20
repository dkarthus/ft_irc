#include "Socket.hpp"

Socket::Socket()
{
	this->port = SERVER_PORT;
	memset(&hints, 0, sizeof(hints));
}

Socket::Socket(const Socket &other)
{

}

Socket &Socket::operator=(const Socket &other)
{
	return (*this);
}

Socket::~Socket()
{

}

const char &Socket::getPort() const
{
	return *port;
}

void Socket::setPort(const char *other_port)
{
	Socket::port = other_port;
}

void Socket::createAddrinfo()
{
	int 		rv;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	// need to address port receival
	if ((rv = getaddrinfo(NULL, this->port, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo server: %s\n", gai_strerror(rv));
		exit(1);
	}
}

void Socket::createSocket()
{
	listen_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
}

addrinfo *Socket::getRes() const
{
	return res;
}

int Socket::getListenSock()
{
	return listen_sock;
}

