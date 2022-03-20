#include "Message.hpp"
#include "queue"

Message::~Message()
{}
Message::Message(){

}
std::queue<std::string>	ssplit(const std::string &s, char sep, bool include)
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

Message::Message(const std::string &str)
{
    std::string	strWithoutNL = std::string(str.begin(), str.end() - 1);
    std::queue<std::string>	que = ssplit(strWithoutNL, ' ', false);
    if (que.size() > 0)
    {
        command = que.front();
        que.pop();
    }
    while (que.size() > 0)
    {
        //избавляем от :
        if (que.front()[0] == ':')
        {
            std::string	s(que.front().begin() + 1, que.front().end());
            que.pop();
            while (que.size() > 0)
            {
                s.append(" ");
                s.append(que.front());
                que.pop();
            }
            parameters.push_back(s);
        }
        else
        {
            parameters.push_back(que.front());
            que.pop();
        }
    }
}

const std::string	&Message::getCommand() const{
    return command;
}

const std::vector<std::string>	&Message::getParameters() const
{
    return parameters;
}


