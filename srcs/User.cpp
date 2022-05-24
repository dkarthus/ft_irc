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
    std::cout<<mssg.c_str() << "to fd "<< sockfd << std::endl;
}

User::User(int sockfd): sockfd(sockfd), flags("") {}

User::User(int sockfd, const std::string& nName):
sockfd(sockfd), nickname(nName), flags("") {}

User::User(int sockfd, const std::string& nName, const std::string &hName):
sockfd(sockfd), nickname(nName), hostname(hName), flags("") {}

User::User(int sockfd, const std::string& nName, const std::string& hName, const std::string& uName):
sockfd(sockfd), nickname(nName), hostname(hName), username(uName), flags("") {}

User::User(int sockfd, const std::string& nName, const std::string& hName, const std::string& uName, const std::string& rName):
sockfd(sockfd), nickname(nName), hostname(hName), username(uName), realname(rName), flags("") {}

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

void	User::setFlag(const std::string &flag)
{
	std::string::size_type n;
	if ((n = flags.find(flag)) != std::string::npos)
		return;
	else
		flags += flag;
}

void	User::removeFlag(const std::string &flag)
{
	std::string::size_type n;
	if ((n = flags.find(flag)) == std::string::npos)
		flags.erase(n);
}

const std::string	&User::getFlags() const
{
	return flags;
}


bool	User::containsFlag(const std::string &flag) const
{
	if (flags.find(flag) != std::string::npos)
		return true;
	return false;
}

int		User::processFlags(const Message &msg, User &user)
{
	std::string	flag = msg.getParameters()[1];
	if (flag == "+i")
		setFlag("i");
	else if (flag == "-i")
		removeFlag("i");
	else if (flag == "+s")
		setFlag("s");
	else if (flag == "-s")
		removeFlag("s");
	else if (flag == "+w")
		setFlag("w");
	else if (flag == "-w")
		removeFlag("w");
	else if (flag == "+o")
		setFlag("o");
	else if (flag == "-o")
		removeFlag("o");
	else
		return responser.sendError(user.getSockfd(), ERR_UMODEUNKNOWNFLAG, "");
	return 0;
}

//int getFdNick(std::string Nick){
//
//}

