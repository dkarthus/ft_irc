#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

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
#include "User.hpp"
#define BUFFSIZE 		510

//int 		sendPrivmsg(int fd, const std::vector<std::string> param, int fd_send, std::string sender, User *user);

#endif