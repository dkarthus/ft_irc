#include "Responser.hpp"

Responser::Responser()
{
	this->motdstart = ":IRCat 375 Wiz :- IRCat Message of the day - \n";
	this->motd = ":IRCat 372 Wiz :- IRC Welcome to server!!!\n";
	this->endofmotd = ":IRCat 376 Wiz :End of /MOTD command";
	this->MOTD.push_back(this->motdstart);
	this->MOTD.push_back(this->motd);
	this->MOTD.push_back(this->endofmotd);



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

std::string 		Responser::getMotd() const
{
	return this->motd;
}

void 			Responser::sendMotd(int fd) const
{
	for (int i = 0; i < 3; ++i)
	{
		send(fd, this->MOTD[i].c_str(), this->MOTD[i].length(), 0);
	}
}

void Responser::sendResponse(int fd, int respCode)
{
	std::stringstream	ss;
	ss << respCode;
	std::string 		serverName = "IRCSERV";
	std::string 		response = ":" + serverName + " " + ss.str() + " " + "Wiz" + " :";
	std::cout << "Printing response" << response << std::endl;
//	switch (respCode)
//	{
//		case RPL_MOTDSTART:
//			std::cout << response;
//			break;
//		case RPL_MOTD:
//			send(fd, this->motd.c_str(), this->motd.length(), 0);
//			break;
//		case RPL_ENDOFMOTD:
//			send(fd, this->endofmotd.c_str(), this->endofmotd.length(), 0);
//			break;
//	}
	send(fd, response.c_str(), response.length(), 0);
}

void Responser::processResponse()
{

}