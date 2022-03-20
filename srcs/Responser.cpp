#include "Responser.hpp"
#include <sstream>
#include <iostream>

Responser::Responser()
{
}

Responser::Responser(const Responser &other)
{

}

Responser &Responser::operator=(const Responser &other)
{
	return *this;
}

Responser::~Responser()
{

}

void Responser::sendResponse(int fd, int respCode)
{
	std::stringstream	ss;
	ss << respCode;
	std::string 		serverName = "IRCSERV";
	std::string 		nick;
	std::string 		response = ":" + serverName + " " + ss.str() + " " + "Wiz" + " :";
	switch (respCode)
	{
		case RPL_MOTDSTART:
			response = response + "- IRCSERV Message of the day - \n";
			break;
		case RPL_MOTD:
			response = response + "- IRC Welcome to server!!!\n";
			break;
		case RPL_ENDOFMOTD:
			response = response + "End of /MOTD command\n";
			break;
		case RPL_AWAY:
			response = response + nick + "  : the user is away\n";
	}
//	std::cout << "Printing response" << response << std::endl;
	send(fd, response.c_str(), response.length(), 0);
}

void 			Responser::sendMotd(int fd)
{
	this->sendResponse(fd, RPL_MOTDSTART);
	this->sendResponse(fd, RPL_MOTD);
	this->sendResponse(fd, RPL_ENDOFMOTD);
}

void Responser::sendError(int fd, int errorCode, const std::string& command)
{
	std::string 		error;
	std::string 		serverName = "IRCSERV";
	std::stringstream	ss;
	std::string 		nick;
	std::string 		channelName;
	ss << errorCode;

	error = ':' + serverName + " " + ss.str() + " ";
	switch (errorCode)
	{
		case ERR_NEEDMOREPARAMS:
			error = error + command + " :Not enough parameters";
			break;
		case ERR_NONICKNAMEGIVEN:
			error = error + ":No nickname given";
			break;
		case ERR_NICKNAMEINUSE:
			error = error + nick + " :Nickname is already in use";
			break;
		case ERR_ERRONEUSNICKNAME:
			error = error + nick + " :Erroneus nickname";
			break;
		case ERR_NICKCOLLISION:
			error = error + nick + " :Nickname collision KILL";
			break;
		case ERR_ALREADYREGISTRED:
			error = error + ":You may not reregister";
			break;
		case ERR_NORECIPIENT:
			error = error + ":No recipient given " + command;
			break;
		case ERR_CANNOTSENDTOCHAN:
			error = error + channelName + "  :Cannot send to channel";
			break;
		case ERR_NOSUCHNICK:
			error = error + nick + " No such nick/channel";
			break;
		case ERR_NOTEXTTOSEND:
			error = error + ":No text to send";
			break;
	}
	std::cout << "Printing error" << error << std::endl;
	send(fd, error.c_str(), error.length(), 0);
}
