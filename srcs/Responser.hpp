#ifndef RESPONSER_HPP
#define RESPONSER_HPP

#include <sstream>
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
#include "utils.hpp"
#define RPL_MOTD		372
#define RPL_MOTDSTART	375
#define RPL_ENDOFMOTD	376
#define SERVER_NAME		"IRCSERV"

class Responser {
private:
	std::string 		motdstart;
	std::string 		motd;
	std::string 		endofmotd;
	std::vector<std::string>		MOTD;

public:
	Responser();
	Responser(const Responser &other);
	Responser		&operator=(const Responser &other);
	~Responser();

	std::string 		getMotd() const;
	void 				sendMotd(int fd) const;
	void				processResponse();
	void				sendResponse(int fd, int response);



};

#endif