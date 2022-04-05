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
#include <sys/ioctl.h>
#include <fcntl.h>
#include "Socket.hpp"
#include "Storage.hpp"
#include "Responser.hpp"
#include "User.hpp"
#include "Privmsg.hpp"
#include "Message.hpp"
#define TRUE 1
#define FALSE 0
#define FD_SIZE 10


class Server
{
private:
    Socket				servSocket;
    Responser           responser;
    Storage				storage[FD_SIZE];
    User 				user;
public:
	Server();

    struct pollfd		fds[FD_SIZE];
    int                 nfds;
    int 				timeout;

    int 				currentSize;

    Server(const char *port);
    Server(const Server &other);
    const Socket    &getServSocket() const;
    Server			&operator=(const Server &other);
	~Server();
	void			startSocket(Socket &serv_socket);
	void 			listenConnections(int socket);

	void 			initFdStruct(int socket);
	int 			pollConnections(int socket);
    int				check_error(const std::string message, std::vector<std::string> param);
	void			printFds();

};

#endif