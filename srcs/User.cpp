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

    // std::string str = getMessage()
    

    // std::string	strWithoutNL = std::string(str.begin(), str.end() - 1);
    // std::queue<std::string>	que = split(strWithoutNL, ' ', false);
    // if (que.size() > 0 && que.front()[0] == ':')
    // {
    //     prefix = std::string(que.front().begin() + 1, que.front().end());
    //     que.pop();
    // }
    // if (que.size() > 0)
    // {
    //     command = que.front();
    //     que.pop();
    // }
    // while (que.size() > 0)
    // {
    //     if (que.front()[0] == ':')
    //     {
    //         std::string	s(que.front().begin() + 1, que.front().end());
    //         que.pop();
    //         while (que.size() > 0)
    //         {
    //             s.append(" ");
    //             s.append(que.front());
    //             que.pop();
    //         }
    //         parameters.push_back(s);
    //     }
    //     else
    //     {
    //         parameters.push_back(que.front());
    //         que.pop();
    //     }
    // }
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
