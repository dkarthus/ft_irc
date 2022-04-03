#pragma once

#include <string>
#include <vector>
#include <algorithm>


class Message
{
private:
    std::string						command;
    std::vector<std::string>		parameters;

//    Message();
    Message(const Message& copy);
    Message	&operator=(const Message& other);
public:
    Message();
    Message(const std::string &str);
    virtual ~Message();
    const std::string				&getCommand() const;
    const std::vector<std::string>	&getParameters() const;
};
