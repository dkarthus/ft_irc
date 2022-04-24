//
// Created by yuliya on 2/16/22.
//
#ifndef USER_HPP
#define USER_HPP

#include "Storage.hpp"
#include "Socket.hpp"
#include "Message.hpp"
#include "Channel.hpp"
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

#define REGISTERED		0b00000001
#define IRC_NOSIGNAL SO_NOSIGPIPE


class User {
public:
    //Storage				                storage;
    //std::string						    prefix;
    //std::string						    command;
    //std::vector<std::string>		    parameters;

	User(int sockfd);
    User(int sockfd, const std::string& nName);
    User(int sockfd, const std::string& nName, const std::string& hName);
    User(int sockfd, const std::string& nName, const std::string& hName, const std::string& uName);
    User(int sockfd, const std::string& nName, const std::string& hName, const std::string& uName, const std::string& rName);
	User();
	~User();
    unsigned char						flags;

    //setters
	void								setPassword(const std::string &pass);
	void								setUsername(const std::string &uName);
	void								setHostname(const std::string &hName);
	void								setNickname(const std::string &nName);
	void								setRealname(const std::string &rName);
    void                                setFlag(unsigned char flag);

    //geters
    const std::queue<std::string>		&getMessages() const;
    const std::string					&getUsername() const;
    const std::string					&getHostname() const;
	const std::string					&getNickname() const;
	const std::string					&getRealname() const;
	const std::string					&getPassword() const;
    int	                                getSockfd() const;

    //methods
    void								parse_message(Node *mes);
	void								sendMessage(const std::string &mssg) const;
    

	std::queue<std::string>				message;
    unsigned char	getFlags() const;
private:
    int                                 sockfd;
    std::string							password;
    std::string							nickname;
    std::string							username;
    std::string							hostname;
    std::string                         realname;
};

#endif
