#ifndef FT_IRC_SERVER_H
#define FT_IRC_SERVER_H
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cerrno>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "Socket.hpp"
#include "Storage.hpp"
#include "Responser.hpp"
#include "User.hpp"
#include "Privmsg.hpp"
#include "Message.hpp"
#define TRUE 1
#define FALSE 0
#define FD_SIZE 10
#define	DISCONNECT	-2

class Channel;
class User;

class Server
{
private:
    Socket				servSocket;
    Responser           responser;
    Storage				storage[FD_SIZE];
    std::vector<User *> users;


public:
    Server();

	std::vector<struct pollfd>	fds_vec;
    struct pollfd		fds[FD_SIZE];
    int                 nfds;
    int 				timeout;

    int 				currentSize;
    std::map<std::string, Channel *> channels;
    Server(const char *port);
    Server(const Server &other);
    const Socket    &getServSocket() const;
    Server			&operator=(const Server &other);
    ~Server();
    void			startSocket(Socket &serv_socket);
    void 			listenConnections(int socket);

    void 			initFdStruct(int socket);
    int 			pollConnections(int socket);
    // int				check_error(const std::string message, std::vector<std::string> param, int i);
    void			printFds();
    int				set_param_user(const std::string message, std::vector<std::string> param, int i);
    int checkConnection(int n, int fd, int i);
    int getFdByNick(const  std::string nick);
    std::string getNickbyFd(int fd);
    int validName(std::string params);
    void createChannel(std::string name, std::string pass, User *user);
    void inviteChannel(std::vector<std::string> params, User *user, int fd);
    int sendPrivmsg(int fd, const std::vector<std::string> param, int fd_send, std::string sender, User *user);
//    int ExCommands(Message *msg, User *user, int i);
    User *getUserByName(const std::string &name);

	bool	containsChannel(const std::string &name) const;
	bool	containsUser(const std::string &nick) const;
};
#endif