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

#define ERR_NEEDMOREPARAMS	461
#define ERR_NONICKNAMEGIVEN	431
#define ERR_NICKNAMEINUSE	433
#define ERR_ERRONEUSNICKNAME	432
#define ERR_NICKCOLLISION	436
#define ERR_ALREADYREGISTRED	462
#define ERR_NORECIPIENT		411
#define ERR_CANNOTSENDTOCHAN	404
#define ERR_NOSUCHNICK		401
#define ERR_NOTEXTTOSEND		412
#define ERR_NOTREGISTERED	451
#define ERR_NOSUCHCHANNEL	403
#define ERR_BADCHANNELKEY	475
#define ERR_USERONCHANNEL	443
#define ERR_CHANOPRIVSNEEDED	482
#define ERR_NOTONCHANNEL		442
#define ERR_USERNOTINCHANNEL	441

#define RPL_NOTOPIC			331
#define RPL_NAMREPLY			353
#define RPL_ENDOFNAMES		366
#define RPL_MOTD				372
#define RPL_MOTDSTART		375
#define RPL_ENDOFMOTD		376
#define RPL_AWAY				301
#define RPL_INVITING			341


class Responser {
private:

public:
	Responser();
	Responser(const Responser &other);
	Responser		&operator=(const Responser &other);
	~Responser();

	void 				sendMotd(int fd, std::string &nick);
	void				processResponse();
	void				sendResponse(int fd, int response, std::string &nick);
	int				    sendError(int fd, int error, const std::string& command);
    int                 sendAnswerJoin(int fd, int errorCode, const std::string& nick, const std::string& name);
    int                 sendListUsers(int fd, int errorCode, const std::string& nick, const std::string& nicksUser, const std::string& name);
};

#endif