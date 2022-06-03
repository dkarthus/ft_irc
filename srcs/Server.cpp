#include "Server.hpp"

Server::Server()
{

}

Server::Server(const char *port, std::string p)
{
    servSocket = Socket(port);
    servSocket.createAddrinfo();
    startSocket(servSocket);
    listenConnections(servSocket.getListenSock());
    initFdStruct(servSocket.getListenSock());
    pass = p;
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

int Server::checkNick(std::string nick)
{
    for (size_t i = 0; i < users.size(); i++)
        if (users[i]->getNickname() == nick)
            return -1;
    return 1;
}

int	Server::set_param_user(const std::string command, std::vector<std::string> param, int i) {
    int pass = 0;
    if (command == "PASS"){
        pass = 1;
        if (this->pass != param[0])
            return(responser.sendError(fds_vec[i].fd, ERR_PASSWDMISMATCH , "PASS"));
        users[i]->setPassword(param[0]);
    }
    else if (command == "USER")
        users[i]->setUsername(param[0]);
    else if (command == "NICK") {
        if (checkNick(param[0])== -1)
            return(responser.sendError(fds_vec[i].fd, ERR_NICKCOLLISION, "NICK"));
        users[i]->setNickname(param[0]);
    }
    return pass;
//    if (user.getMessage().size > 0)
//        return (NOTREGISTERED);

}

int	Server::checkConnection(int n, int fd, int i) {
    if (!users[i]->getNickname().empty() && !users[i]->getUsername().empty()){
        if (users[i]->getPassword().size() > 0 || n == 1){
            if (!(users[i]->containsFlag("r"))){
                users[i]->setFlag("r");
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

int Server::validName(std::string params){
    if (params[0] != '#')
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
void Server::createChannel(std::string name, std::string pass, User *user){

//    if (pass.size() == 0)
//        pass = "";
    channels[name] = new Channel(name, user, pass);
    std::cout << "@" << user->getNickname() <<" created Channel " << name << std::endl;
}

std::queue<std::string>	split4(const std::string &s, char sep, bool include)
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


int Server::pollConnections(int listenSocket) {
    int rc;
    int i;
    int newSocket;
    int closeConn;
    // Responser response;
	struct pollfd new_fd;
//    int bot = 0;

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
//                   ExCommands(&msg, users[i-1], i);
                   if (msg.getCommand() == "JOIN") {
                       if (msg.getParameters().size() == 0)
                           return (responser.sendError(fds_vec[i].fd, ERR_NEEDMOREPARAMS, msg.getCommand()));
                       std::queue<std::string> keys;
                       std::string secPar = "";
                       std::queue<std::string> chans = split4(msg.getParameters()[0], ',', false);

					   if (msg.getParameters().size() == 2)
						   secPar = msg.getParameters()[1];
                       keys = split4(secPar, ',', false);
                       for (; chans.size() > 0; chans.pop()) {
                           std::string	key = keys.size() ? keys.front() : "";
                           if (keys.size() > 0)
                               keys.pop();
                           if (validName(chans.front()) == -1)
                               return (responser.sendError(fds_vec[i].fd, ERR_NOSUCHCHANNEL, msg.getCommand()));
                           if (msg.getParameters().size() == 1)
                               key = "";
                           try {
                               Channel *ch = channels.at(chans.front());
                               ch->CheckConnect(users[i - 1], key);
                           }
                           catch (const std::exception &e) {
                               createChannel(chans.front(),key , users[i - 1]);
                           }
                       }
                   }
//                   else if (msg.getCommand() == "BOT")
//                   {
////                       User *Bot = new User( newSocket + 10);
//                       sendPrivmsg(fds_vec[i].fd, msg.getParameters(), fds_vec[i].fd, "bot", users[i - 1]);
//
//
//                   }
//                   else if  (msg.getCommand() == "INVITE") {
//                       inviteChannel(msg.getParameters(), users[i-1]);
//                   }
                   else if (msg.getCommand() == "PRIVMSG" || msg.getCommand() == "NOTICE" ){
                       if (msg.getParameters().size() == 0)
                           return(responser.sendError(fds_vec[i].fd, ERR_NORECIPIENT, msg.getCommand()));
                       else if (msg.getParameters().size() == 1)
                           return(responser.sendError(fds_vec[i].fd, ERR_NOTEXTTOSEND, msg.getCommand()));
                       else if (msg.getParameters()[0] == "bot")
                           sendPrivmsgToBot(fds_vec[i].fd, msg.getParameters()[1],getNickbyFd(fds_vec[i].fd),  users[i - 1]);
//                       else if (getNickbyFd(fds_vec[i].fd) )
//                           return(responser.sendError(fds_vec[i].fd, ERR_NORECIPIENT, msg.getCommand()));
                       else
                            sendPrivmsg(fds_vec[i].fd, msg.getParameters(), getFdByNick(msg.getParameters()[0]), getNickbyFd(fds_vec[i].fd), users[i - 1]);
                   }
                    else if (msg.getCommand() == "QUIT"){
                        std::vector<User*>::iterator it = users.begin();
                        users.erase(it + i - 1);
//                        users[i-1]->removeFlag("r");
                    }
//                   else if (msg.getCommand() == "WHO"){
//                       std::string myMessage = ":IRCSERV 315 kalexand kalexand :End of /WHO list";
//                       send(fds_vec[i].fd, myMessage.c_str(), myMessage.size(), 0);
//                   }
//                    else if (msg.getCommand() == "KICK")
//                    {
// //                       commandKICK();
//                        Channel	*chan = channels.at(msg.getParameters()[0]);
//                        std::string	message = "KICK " + chan->getName() + " " + msg.getParameters()[1] + " :";
//                        if (msg.getParameters().size() > 2)
//                            message += msg.getParameters()[2];
//                        else
//                            message += users[i - 1]->getNickname();
//                        chan->sendMessage(message + "\n", users[i - 1], true);
//                        chan->disconnect(*(getUserByName(msg.getParameters()[1])));
//                        getUserByName(msg.getParameters()[1])->removeChannel(msg.getParams()[0]);

//                    }
//                   else if (msg.getCommand() == "WHO"){
//                       std::string myMessage = ":IRCSERV 315 kalexand kalexand :End of /WHO list";
//                       send(fds_vec[i].fd, myMessage.c_str(), myMessage.size(), 0);
//                   }
                   else if (msg.getCommand() == "KICK")
                   {
					   if (msg.getParameters().size() < 2)
						   responser.sendError(fds_vec[i].fd, ERR_NEEDMOREPARAMS, "KICK");
					   else if (!containsChannel(msg.getParameters()[0]))
						   responser.sendError(fds_vec[i].fd, ERR_NOSUCHCHANNEL, msg.getParameters()[0]);
					   else if (!channels.at(msg.getParameters()[0])->isOperator(*users[i - 1]))
						   responser.sendError(fds_vec[i].fd, ERR_CHANOPRIVSNEEDED, msg.getParameters()[0]);
					   else if (!channels.at(msg.getParameters()[0])->isMember(users[i - 1]->getNickname()))
						   responser.sendError(fds_vec[i].fd, ERR_NOTONCHANNEL, msg.getParameters()[0]);
					   else if (!channels[msg.getParameters()[0]]->isMember(msg.getParameters()[1]))
						   responser.sendError(fds_vec[i].fd, ERR_NOSUCHNICK, msg.getParameters()[1]);
					   else if (!channels.at(msg.getParameters()[0])->isMember(msg.getParameters()[1]))
						   responser.sendError(fds_vec[i].fd, ERR_USERNOTINCHANNEL, msg.getParameters()[1]);
					   else
					   {
						   Channel *chan = channels.at(msg.getParameters()[0]);
						   std::string message = "KICK " + chan->getName() + " " + msg.getParameters()[1] + " :";
						   if (msg.getParameters().size() > 2)
							   message += msg.getParameters()[2];
						   else
							   message += users[i - 1]->getNickname();
						   chan->sendMessageKick(users[i - 1], msg.getParameters()[1], message + "\n");
						   chan->removeUser(*(getUserByName(msg.getParameters()[1])));
					   }
                   }
				   else if (msg.getCommand() == "INVITE")
				   {
					   if (msg.getParameters().size() < 2)
						   responser.sendError(fds_vec[i].fd, ERR_NEEDMOREPARAMS, " INVITE " + users[i - 1]->getNickname());
					   else if (!containsUser(msg.getParameters()[0]))
						   responser.sendError(fds_vec[i].fd, ERR_NOSUCHNICK, msg.getParameters()[0]);
					   else if (!containsChannel(msg.getParameters()[1]))
						   responser.sendError(fds_vec[i].fd, ERR_NOTONCHANNEL, msg.getParameters()[1]);
					   else
					   {
						   User *recipient = getUserByName( msg.getParameters()[0]);
						   Channel *chan = channels.at(msg.getParameters()[1]);
						   if (chan->isMember(msg.getParameters()[0]))
							   responser.sendError(fds_vec[i].fd, ERR_USERONCHANNEL, msg.getParameters()[0]);
						   else
						   {
							   if (chan->containsFlag("i") && chan->isOperator(*users[i - 1]))
									responser.sendError(fds_vec[i].fd, ERR_CHANOPRIVSNEEDED, users[i - 1]->getNickname());
							   else
							   {
								   chan->addUser(*getUserByName( msg.getParameters()[0]));
								   recipient->sendMessage(":" + users[i - 1]->getNickname() + "!Adium@127.0.0.1 " + "INVITE " + recipient->getNickname
								   () + " :" + chan->getName() + "\n");
								   chan->sendMessageInvite(users[i - 1], RPL_INVITING, "localhost", chan->getName(),
														   recipient->getNickname());
								   chan->sendMessageJoin(getUserByName( msg.getParameters()[0]), chan->getName(), "JOIN");
								   responser.sendAnswerJoin(getUserByName( msg.getParameters()[0])->getSockfd(),
															RPL_NOTOPIC, getUserByName( msg.getParameters()[0])->getNickname(), chan->getName());
								   chan->answerOper(getUserByName( msg.getParameters()[0]));
								   responser.sendAnswerJoin(getUserByName( msg.getParameters()[0])->getSockfd(), RPL_ENDOFNAMES, getUserByName( msg.getParameters()[0])->getNickname(), chan->getName());
							   }
						   }
					   }
				   }
				   else if (msg.getCommand() == "MODE") {
					   User *recipient;
					   Channel *chan;

					   if (msg.getParameters().empty())
					   {
						   responser.sendError(fds_vec[i].fd, ERR_NEEDMOREPARAMS, " MODE " + users[i - 1]->getNickname
								   ());
						   break;
					   }
					   User *op = getUserByName(getNickbyFd(fds_vec[i].fd));
					   if (msg.getParameters()[0][0] == '#')
					   {
						   chan = channels.at(msg.getParameters()[0]);
						   if (msg.getParameters().size() > 2)
							   recipient = getUserByName(msg.getParameters()[2]);
						   if (!containsChannel(msg.getParameters()[0]))
							   responser.sendError(fds_vec[i].fd, ERR_NOSUCHCHANNEL, msg.getParameters()[0]);
						   else if (msg.getParameters().size() == 1)
						   {
							   rpl("!Adium@127.0.0.1", *op, RPL_CHANNELMODEIS, msg.getParameters()[0], channels.at
									   (msg.getParameters()[0])->getFlags());
							   break;
						   }
						   else if (!channels.at(msg.getParameters()[0])->isOperator(*op))
							   responser.sendError(fds_vec[i].fd, ERR_CHANOPRIVSNEEDED, msg.getParameters()[0]);
						   else if (!channels.at(msg.getParameters()[0])->isMember(recipient->getNickname()) ||
						   !channels.at(msg.getParameters()[0])->isMember(op->getNickname()))
							   responser.sendError(fds_vec[i].fd, ERR_NOTONCHANNEL, msg.getParameters()[0]);
						   else if (chan->processFlags(msg, *op, *recipient) != -1)
						   {
							   std::string	flag = msg.getParameters()[1];
							   std::string	tmp = (flag[1] == 'o' || flag[1] == 'v') ? " " + msg.getParameters()[2] : "";
							   channels.at(msg.getParameters()[0])->sendMessageMode("MODE " + msg.getParameters()[0] + " " + msg.getParameters()[1]  + tmp + "\n", *op);
						   }
					   }
					   else
					   {
						   if (!(op->getNickname() == msg.getParameters()[0]))
						   	   responser.sendError(fds_vec[i].fd, ERR_USERSDONTMATCH, "");
						   else
						   {
							   if (msg.getParameters().size() == 1)
							   {
								   std::string	flags = "+";
								   if (!op->containsFlag("i"))
									   flags += "i";
								   if (!op->containsFlag("s"))
									   flags += "s";
								   if (!op->containsFlag("w"))
									   flags += "w";
								   if (!op->containsFlag("o"))
									   flags += "o";
								   rpl("!Adium@127.0.0.1", *op, RPL_UMODEIS, flags, "");
							   }
							   else if (op->processFlags(msg, *op) != -1)
								   op->sendMessage(":" + users[i - 1]->getNickname() + "!Adium@127.0.0.1 " + "MODE " + msg.getParameters()[0] + " " +
								   msg.getParameters()[1] + "\n");
						   }
					   }
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
//                std::cout << "Printing data" << std::endl;
//                storage[i].printNodes();
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

User	*Server::getUserByName(const std::string &name)
{
    User	*ret = NULL;
    size_t	usersCount = users.size();
    for (size_t i = 0; i < usersCount; i++)
        if (users[i]->getNickname() == name)
            ret = users[i];
    return ret;
}

bool	Server::containsChannel(const std::string &name) const
{
	try
	{
		channels.at(name);
		return true;
	}
	catch(const std::exception& e)
	{}
	return false;
}

bool	Server::containsUser(const std::string &nick) const
{
	size_t	usersCount = users.size();
	for (size_t i = 0; i < usersCount; i++)
	{
		if (users[i]->getNickname() == nick)
			return (true);
	}
	return (false);
}

int		Server::rpl(const std::string &from, const User &user, int rpl, const std::string &arg1,const std::string &arg2)
{
	std::string	msg = ":" + from + " ";
	std::stringstream	rplStr;
	rplStr << rpl;
	msg += rplStr.str() + " " + user.getNickname() + " " + arg1 + " +" + arg2 + "\n";
	std::cout << msg << std::endl;
	send(user.getSockfd(), msg.c_str(), msg.size(), IRC_NOSIGNAL);
	return 0;
}