#include "User.hpp"

#include <iostream>
User::User()
{

}

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

const std::queue<std::string>	&User::getMessage() const
{
	return message;
}

//
//void User::setPassword(std::string password){
//    this->password = password;
//}
//void User::setNickname(std::string nickname){
//    this->nickname = nickname;
//}
//void User::setHostname(std::string hostname){
//    this->hostname = hostname;
//}
