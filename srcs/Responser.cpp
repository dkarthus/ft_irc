#include "Responser.hpp"

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

std::string 		Responser::getMotd() const
{
    return this->motd;
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

void 			Responser::sendMotd(int fd)
{
    this->sendResponse(fd, RPL_MOTDSTART);
    this->sendResponse(fd, RPL_MOTD);
    this->sendResponse(fd, RPL_ENDOFMOTD);
}