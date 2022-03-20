//
// Created by yuliya on 2/16/22.
//
#ifndef USER_HPP
#define USER_HPP

#include "Storage.hpp"
#include "Socket.hpp"
#include "Message.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h> // For socket functions
#include <unistd.h>
#include <utility>
#include <vector>
#include <queue>
#include <algorithm>


class User {
public:
    std::queue<std::string>				message;
    Storage				                storage;
    std::string						    prefix;
    std::string						    command;
    std::vector<std::string>		    parameters;


    User();
    void parse_message(Node *mes);
    const std::queue<std::string>   &getMessage() const;
    void setPassword(std::string password);
    void setNickname(std::string nickname);
    void setHostname(std::string hostname);
//    void setParametrs(const Message &msg);

    std::string							password;
    std::string							nickname;
    std::string							username;
    std::string							hostname;


};

#endif
