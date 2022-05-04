#include "User.hpp"

#include <iostream>


std::queue<std::string>	split(const std::string &s, char sep, bool include)
{
    std::queue<std::string>	ret;
    std::string::const_iterator	i = s.begin();
    while(i != s.end())
    {
        while (i != s.end() && *i == sep)
            ++i;
        std::string::const_iterator	j = std::find(i, s.end(), sep);
        if (i != s.end())
        {
            if (include && j != s.end())
                ret.push(std::string(i, j + 1));
            else
                ret.push(std::string(i, j));
            i = j;
        }
    }
    return ret;
}

void User::parse_message(Node *mes) {
    std::string text = mes->getCommand();
    if (text.length() > 512)
        text = text.substr(0, 510) + "\r\n";
    while (text.find("\r\n") != std::string::npos)
        text.replace(text.find("\r\n"), 2, "\n");
    if (text.size() > 1)
        message = split(text, '\n', true);
}

void	User::sendMessage(const std::string &mssg) const
{
	if (mssg.size() > 0)
		send(sockfd, mssg.c_str(), mssg.size(), IRC_NOSIGNAL);
    std::cout << mssg << std::endl;
}

User::User(int sockfd): sockfd(sockfd) {}

User::User(int sockfd, const std::string& nName):
sockfd(sockfd), nickname(nName) {}

User::User(int sockfd, const std::string& nName, const std::string &hName):
sockfd(sockfd), nickname(nName), hostname(hName) {}

User::User(int sockfd, const std::string& nName, const std::string& hName, const std::string& uName):
sockfd(sockfd), nickname(nName), hostname(hName), username(uName) {}

User::User(int sockfd, const std::string& nName, const std::string& hName, const std::string& uName, const std::string& rName):
sockfd(sockfd), nickname(nName), hostname(hName), username(uName), realname(rName) {}

User::~User()
{}

int		User::getSockfd() const
{
	return sockfd;
}

const std::string	&User::getUsername() const
{
	return username;
}

const std::string	&User::getHostname() const
{
	return hostname;
}

const std::string	&User::getNickname() const
{
	return nickname;
}

const std::string	&User::getRealname() const
{
	return realname;
}

const std::string	&User::getPassword() const
{
	return password;
}

void	User::setPassword(const std::string &pass)
{
	password = pass;
}

void	User::setUsername(const std::string &uName)
{
    username = uName;
}

void	User::setHostname(const std::string &hName)
{
	hostname = hName;
}

void	User::setNickname(const std::string &nName)
{
	nickname = nName;
}

void	User::setRealname(const std::string &rName)
{
	realname = rName;
}

User::User()
{

}

const std::queue<std::string> &User::getMessages() const
{
	return this->message;
}


unsigned char	User::getFlags() const
{
	return flags;
}

void	User::setFlag(unsigned char flag)
{
	flags |= flag;
	// if (flag == BREAKCONNECTION && quitMessage.size() == 0)
	// 	quitMessage = "Client exited";
}

//int getFdNick(std::string Nick){
//
//}

