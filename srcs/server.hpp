#ifndef FT_IRC_SERVER_H
#define FT_IRC_SERVER_H
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cerrno>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <string>
//#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "Socket.hpp"
#include "Storage.hpp"
#include "Responser.hpp"
#include "utils.hpp"
#include "User.hpp"
#define SERVER_PORT "3490"
#define TRUE 1
#define FALSE 0
#define FD_SIZE 10
#include "Message.hpp"

bool killswitch = true;

class Server
{
private:
	struct pollfd		fds[FD_SIZE];
	int 				timeout;
	Storage				storage[FD_SIZE];
	std::vector<User*>	user;

public:
	Server();
	Server(const Server &other);
	Server			&operator=(const Server &other);
	~Server();
	void			startSocket(Socket &serv_socket);

	void 			listenConnections(int socket);
	void 			initFdStruct(int socket);
	void 			pollConnections(int socket);
    int	check_error(const std::string message);
	void			printFds();

};

#endif