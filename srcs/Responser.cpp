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
	}
//	std::cout << "Printing response" << response << std::endl;
	send(fd, response.c_str(), response.length(), 0);
}