#include "Server.hpp"

Server::Server()
{

}

Server::Server(const char *port)
{
    servSocket = Socket(port);
    servSocket.createAddrinfo();
    startSocket(servSocket);
    listenConnections(servSocket.getListenSock());
    initFdStruct(servSocket.getListenSock());
}

Server::Server(const Server &other)
{

}

Server &Server::operator=(const Server &other)
{
    return (*this);
}

Server::~Server()
{
    /*************************************************************/
    /* Clean up all of the sockets that are open                 */
    /*************************************************************/
//    for (int i = 0; i < currentSize; i++)
//    {
//        if(fds[i].fd >= 10)
//            close(fds[i].fd);
//    }
    fds_vec.clear();
}

void Server::startSocket(Socket &serv_socket)
{
    struct addrinfo			*tmp;
    int 					listen_sd, rc;
    int 					yes;
    int 					on = 1;

    for (tmp = serv_socket.getRes(); tmp != NULL; tmp->ai_next)
    {
        /*************************************************************/
        /* Create an AF_INET6 stream socket to receive incoming      */
        /* connections on                                            */
        /*************************************************************/
        serv_socket.createSocket();
        listen_sd = serv_socket.getListenSock();
        if (listen_sd < 0)
            continue;
        /*************************************************************/
        /* Allow socket descriptor to be reuseable                   */
        /*************************************************************/
        rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (rc < 0)
        {
            fprintf(stderr, "setsocketopt failed: %s\n", gai_strerror(rc));
            close(listen_sd);
            exit(1);
        }
        /*************************************************************/
        /* Set socket to be nonblocking. All of the sockets for      */
        /* the incoming connections will also be nonblocking since   */
        /* they will inherit that state from the listening socket.   */
        /*************************************************************/
        rc = fcntl(listen_sd, F_SETFL, O_NONBLOCK);
        if (rc < 0)
        {
            perror("fcntl() failed");
            close(listen_sd);
            exit(1);
        }
        /*************************************************************/
        /* Bind the socket                                           */
        /*************************************************************/
        if ((bind(listen_sd, tmp->ai_addr, tmp->ai_addrlen)) < 0)
        {
            close(listen_sd);
            perror("server: bind");
            continue;
        }
        break;
    }
    if (tmp == NULL)
    {
        fprintf(stderr, "selectserver: failed to bind");
        exit(2);
    }
}

void Server::listenConnections(int socket)
{
    int 		rc;
    /*************************************************************/
    /* Set the listen back log                                   */
    /*************************************************************/
    rc = listen(socket, 20);
    if (rc < 0)
    {
        perror("listen() failed");
        close(socket);
        exit(1);
    }
}

void Server::initFdStruct(int socket)
{
    /*************************************************************/
    /* Initialize the pollfd structure                           */
    /*************************************************************/
    memset(fds, 0, sizeof(fds));
    /*************************************************************/
    /* Set up the initial listening socket                        */
    /*************************************************************/
//    fds[0].fd = socket;
//    fds[0].events = POLLIN;
    struct pollfd       fd = {};

    fd.fd = socket;
    fd.events = POLLIN;
    fds_vec.push_back(fd);
    /*************************************************************/
    /* Initialize the timeout to 3 minutes. If no                */
    /* activity after 3 minutes this program will end.           */
    /* timeout value is based on milliseconds.                   */
    /*************************************************************/
    timeout = (30 * 60 * 1000);
}
// int	Server::check_error(const std::string command, std::vector<std::string> param, int i) {
//     if (command != "PASS" && command != "USER" && command != "NICK")
//         return (NOTREGISTERED);
//     else {

// 		/*
//         if (command == "PASS")
//             user.setPassword(param[0]);
//         else if (command == "USER")
//             user.setUsername(param[0]);
//         else if (command == "NICK")
//             user.setNickname(param[0]);
// 		*/
//     }
// }

int	Server::set_param_user(const std::string command, std::vector<std::string> param, int i) {
    int pass = 0;
    if (command == "PASS"){
        pass = 1;
        users[i]->setPassword(param[0]);
    }
    else if (command == "USER")
        users[i]->setUsername(param[0]);
    else if (command == "NICK")
        users[i]->setNickname(param[0]);
    return pass;
//    if (user.getMessage().size > 0)
//        return (NOTREGISTERED);

}

int	Server::checkConnection(int n, int fd, int i) {
    if (users[i]->getNickname().size() > 0 && users[i]->getUsername().size() > 0){
        if (users[i]->getPassword().size() > 0 || n == 1){
            if (!(users[i]->getFlags() & REGISTERED)){
                users[i]->setFlag(REGISTERED);
                Responser response;
//                sendResponse(fd, int respCode, std::string &nick)
                std::string nickTo = users[i]->getNickname();
                response.sendMotd(fd, nickTo);
                // message of the day

            }
        }
        else
            return (DISCONNECT);
    }
    return 0;
}

// int	Server::join(const std::string command, std::vector<std::string> param){
//     std::cout << "JOIN" << " name: " << param[0] << " pass: " << param[1] << std::endl;
// //    if (param.size() == 0)
// //        return -1;
// //    else{
// //        std::queue<std::string>	chans = split3(param[0], ',', false);
// ////        std::cout << chans << std::endl;
// //        std::queue<std::string>	keys;
// //        if (param.size() > 1)
// //            keys = split3(param[1], ',', false);
// //        for (; chans.size() > 0; chans.pop()){
// //                std::string	key = keys.size() ? keys.front() : "";
// //                if (keys.size() > 0)
// //                    keys.pop();
// //
// //        }
// //    }
// //    return 0;

//         if (param.size() == 0)
//             return -1;
//         else{
//             channels[param[0]] = new Channel(param[0], user, param[1]);
//         }
//         return 0;

//     }

int Server::validName(std::vector<std::string> params){
    if (params[0][0] != '#')
        return(-1);
}

void Server::inviteChannel(std::vector<std::string> params, User *user, int fd){
    User	*receiver;
    for (size_t i = 0; i < users.size(); ++i)
        if (users[i]->getNickname() == params[0])
            receiver = users[i];
    Channel	*now = channels.at(params[1]);
//    if (now->containsNickname(params[0]))
//        responser.sendAnswerJoin(fd, ERR_USERONCHANNEL, user->getNickname(), params[1]);


}
void Server::createChannel(std::vector<std::string> params, User *user){

    channels[params[0]] = new Channel(params[0], user, params[1]);
    std::cout << "@" << user->getNickname() <<" created Channel " << params[0] << " with password " << params[1]<< std::endl;
}

int Server::pollConnections(int listenSocket) {
    int rc;
    int i;
    int newSocket;
    int closeConn;
    // Responser response;
	struct pollfd new_fd;

    for (i = 0; i < fds_vec.size(); i++)
    {
        if (fds_vec[i].revents == 0)
            continue;
        if (fds_vec[i].revents != POLLIN) {
            printf("  Error! revents = %d\n", fds_vec[i].revents);
            closeConn = TRUE;
        }
        if (fds_vec[i].fd == listenSocket) {
            newSocket = accept(listenSocket, NULL, NULL);
            if (newSocket < 0)
			{
				if (errno != EWOULDBLOCK)
					perror("  accept() failed");
			}
			else
			{
				//проверка юзера и инициализация юзера
				users.push_back(new User(newSocket));
                struct pollfd       fd = {};
                fd.fd = newSocket;
                fd.events = POLLIN;
				fds_vec.push_back(fd);
			}
        }
        else
        {
            closeConn = FALSE;
            rc = recv(fds_vec[i].fd, storage[i].buffer, sizeof(storage[i].buffer), 0);
            if (rc < 0) {
                if (errno != EWOULDBLOCK) {
                    perror("  recv() failed");
                    closeConn = TRUE;
                }
            }
            if (rc == 0)
            {
                closeConn = TRUE;
                printf("  Connection closed\n");
                printf("  Descriptor %d closed\n", fds_vec[i].fd);
            }
            else
            {
                printf("From fd [%d] received a message '%s'\n", fds_vec[i].fd, storage[i].buffer);
                std::string incomingMSG(storage[i].buffer);
                storage[i].setData();
               users[i-1]->parse_message(storage[i].getData());
               while (users[i-1]->getMessages().size() > 0 &&
						users[i-1]->getMessages().front()[users[i-1]->getMessages().front().size() - 1] == '\n') {
                   //достаем по порядку команды и делим на command и parametrs
                   Message msg(users[i-1]->getMessages().front());
                   //удаляем из user записанный message
                   if (users[i-1]->getMessages().size() > 0)
						users[i-1]->message.pop();
                   if (msg.getCommand() == "JOIN"){
                       if (msg.getParameters().size() == 0)
                           return(responser.sendError(fds_vec[i].fd, ERR_NEEDMOREPARAMS, msg.getCommand()));
                       if (validName(msg.getParameters()) == -1)
                           return(responser.sendError(fds_vec[i].fd, ERR_NOSUCHCHANNEL, msg.getCommand()));
                       try
                       {
                           Channel	*ch = channels.at(msg.getParameters()[0]);
                           ch->CheckConnect(users[i - 1], msg.getParameters()[1]);
                       }
                       catch(const std::exception& e)
                       {
                           createChannel(msg.getParameters(), users[i - 1]);
//                           std::string myMessage = ":IRCSERV 366 kalexand2 #irc :End of /NAMES list\n" ;
//                           send(fds_vec[i].fd, myMessage.c_str(), myMessage.size(), 0);
                       }
                   }
//                   else if  (msg.getCommand() == "INVITE") {
//                       inviteChannel(msg.getParameters(), users[i-1]);
//                   }
                   else if (msg.getCommand() == "PRIVMSG"){

                       if (msg.getParameters().size() == 0)
                           return(responser.sendError(fds_vec[i].fd, ERR_NORECIPIENT, msg.getCommand()));
                       else if (msg.getParameters().size() == 1)
                           return(responser.sendError(fds_vec[i].fd, ERR_NOTEXTTOSEND, msg.getCommand()));

                       sendPrivmsg(fds_vec[i].fd, msg.getParameters(), getFdByNick(msg.getParameters()[0]), getNickbyFd(fds_vec[i].fd), users[i - 1]);
                   }
                   else{
                       int n = 0;
                       n = set_param_user(msg.getCommand(), msg.getParameters(), i-1);
                       checkConnection(n, fds_vec[i].fd, i - 1);
                   }
                   
               }
                // if (incomingMSG.find("NICK") != std::string::npos) {
                //     int sentTo = fds_vec[i].fd;
                //     std::string nickTo = (incomingMSG.find("azat") != std::string::npos) ? ("azat") : ("aizhan");
                //     response.sendMotd(fds_vec[i].fd, nickTo);
                // }
//					sendPrivmsg(fds[i].fd, storage[i].buffer);
                //std::cout << "Printing data" << std::endl;
                //storage[i].printNodes();
//					if (rc < 0) {
//						perror("  send() failed");
//						closeConn = TRUE;
//					}
                //printf("  Has sent data to %d descriptor\n", fds[i].fd);
            }
            if (closeConn) {
                printf("   Closing %d descriptor\n", fds_vec[i].fd);
                close(fds_vec[i].fd);
                fds_vec.erase(fds_vec.begin() + i);
//                fds[i].fd = -1;
//                return (1);
            }
        }
    }  /* End of existing connection is readable             */
    //} /* End of loop through pollable descriptors              */
}

void Server::printFds() {
    for (int i = 0; i < 10; ++i) {
        printf("fd: %d, ", fds[i].fd);
    }
}

const Socket &Server::getServSocket() const {
    return servSocket;
}


int Server::getFdByNick(const std::string nick){
    size_t	usersCount = users.size();
    size_t i = 0;
    int fd = -1;
    if (nick.size() == 0)
        return fd;
    while(usersCount > i){
        if(users[i]->getNickname() == nick)
            fd = users[i]->getSockfd();
        i++;
    }
    return fd;

}

std::string Server::getNickbyFd(int fd){
    size_t	usersCount = users.size();
    size_t i = 0;
    std::string nick = " ";
    while(usersCount > i){
        if(users[i]->getSockfd() == fd)
            nick = users[i]->getNickname();
        i++;
    }
    return nick;
}

