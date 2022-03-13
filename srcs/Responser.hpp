#ifndef RESPONSER_HPP
#define RESPONSER_HPP

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



};

#endif