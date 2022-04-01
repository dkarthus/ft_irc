#include "Server.hpp"



Server::Server()
{

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
	fds[0].fd = socket;
	fds[0].events = POLLIN;
	/*************************************************************/
	/* Initialize the timeout to 3 minutes. If no                */
	/* activity after 3 minutes this program will end.           */
	/* timeout value is based on milliseconds.                   */
	/*************************************************************/
	timeout = (30 * 60 * 1000);
}

int	Server::check_error(const std::string command, std::vector<std::string> param) {
    if (command != "PASS" && command != "USER" && command != "NICK")
        return (NOTREGISTERED);
    else {
        if (command == "PASS")
            user.setPassword(param[0]);
        else if (command == "USER")
            user.setUserName(param[0]);
        else if (command == "NICK")
            user.setNickname(param[0]);
    }
}
    void Server::pollConnections(int listenSocket) {
        int rc;
        int nfds = 1;
        int currentSize;
        int i, j;
        int endServer = FALSE;
        int newSocket;
        int closeConn;
        int len;
        int compressArray = FALSE;
        int count;
        Responser response;


	/*************************************************************/
	/* Loop waiting for incoming connects or for incoming data   */
	/* on any of the connected sockets.                          */
	/*************************************************************/
	for (;;)
	{
		printf("...\n");
		rc = poll(fds, nfds, timeout);
		if (rc <= 0)
		{
			perror("  poll() failed or timed out.  End program.\n");
			break;
		}
		currentSize = nfds;
		for (i = 0; i < currentSize; i++)
		{
			if (fds[i].revents == 0)
				continue;
			if (fds[i].revents != POLLIN)
			{
				printf("  Error! revents = %d\n", fds[i].revents);
                closeConn = TRUE;
			}
			if (fds[i].fd == listenSocket)
			{
//				printf("  Listening socket is readable\n");
				newSocket = accept(listenSocket, NULL, NULL);
				if (newSocket < 0)
				{
					if (errno != EWOULDBLOCK)
						perror("  accept() failed");
				}
				//printf("  New incoming connection - %d\n", newSocket);
				fds[nfds].fd = newSocket;
				fds[nfds].events = POLLIN;
				nfds++;
			}
			else
			{
				//printf("  Descriptor %d is readable\n", fds[i].fd);
				closeConn = FALSE;
				rc = recv(fds[i].fd, storage[i].buffer, sizeof(storage[i].buffer), 0);
				//printf("Printing buffer: %s\n",storage[i].buffer);
                if (rc < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						perror("  recv() failed");
						closeConn = TRUE;
					}
				}
				if (rc == 0)
				{
					closeConn = TRUE;
					printf("  Connection closed\n");
					printf("  Descriptor %d closed\n", fds[i].fd);
				}
				else
				{
					printf("From fd [%d] received a message '%s'\n", fds[i].fd, storage[i].buffer);
					std::string incomingMSG (storage[i].buffer);
					if (incomingMSG.find("PRIVMSG") != std::string::npos) {
						std::cout << "PRIVMSG was detected!" << std::endl;
						// send response to Client:
						int sentTo = fds[i].fd == 4 ? 5 : 4;
						int sentFrom = sentTo == 4 ? 5 : 4;
						std::string nickFrom = (incomingMSG.find("azat") != std::string::npos) ? ("aizhan") : ("azat");
						std::string nickTo = nickFrom == ("azat") ? ("aizhan") : ("azat");
						std::cout << "nickFrom:" << nickFrom << std::endl;
						std::cout << "nickTo:" << nickTo << std::endl;
						std::string myMessage = ":" + nickFrom + "!oem@127.0.0.1" + " PRIVMSG " + nickTo + " :Hello\n";

						std::cout << "TO fd [" << sentTo <<  "] send a message '" << myMessage << "'" << std::endl;
						send(sentTo, myMessage.c_str(), myMessage.length(), 0);
					}
					storage[i].setData();
					len = rc;
					//printf("  %d bytes received\n", len);
//                    response.sendMotd(fds[i].fd);
					//rc = send(fds[i].fd, response.sendMotd().c_str(), len, 0);

					user.parse_message(storage[i].getData());
					while (user.getMessage().size() > 0 &&
						   user.getMessage().front()[user.getMessage().front().size() - 1] == '\n') {
						//достаем по порядку команды и делим на command и parametrs
						Message msg(user.getMessage().front());
						//удаляем из user записанный message
						if (user.getMessage().size() > 0)
							user.message.pop();
						if (check_error(msg.getCommand(), msg.getParameters()) == NOTREGISTERED)
							std::cout << ":You have not registered" << std::endl;
					}
//                    user.setParametrs();
					if (incomingMSG.find("NICK") != std::string::npos)
					{
						int sentTo = fds[i].fd;
						std::string nickTo = (incomingMSG.find("azat") != std::string::npos) ? ("azat") : ("aizhan");
						response.sendMotd(fds[i].fd, nickTo);
					}
//					sendPrivmsg(fds[i].fd, storage[i].buffer);
					//std::cout << "Printing data" << std::endl;
//					storage[i].printNodes();
//					if (rc < 0) {
//						perror("  send() failed");
//						closeConn = TRUE;
//					}
					//printf("  Has sent data to %d descriptor\n", fds[i].fd);
				}
				if (closeConn)
				{
					printf("   Closing %d descriptor\n", fds[i].fd);
					close(fds[i].fd);
					fds[i].fd = -1;
					compressArray = TRUE;
				}
			}
			}  /* End of existing connection is readable             */
		} /* End of loop through pollable descriptors              */
        /***********************************************************/
        /* If the compress_array flag was turned on, we need       */
        /* to squeeze together the array and decrement the number  */
        /* of file descriptors. We do not need to move back the    */
        /* events and revents fields because the events will always*/
        /* be POLLIN in this case, and revents is output.          */
        /***********************************************************/
        if (compressArray) {
            compressArray = FALSE;
            for (i = 0; i < nfds; i++) {
                if (fds[i].fd == -1) {
                    for (j = i; j < nfds; j++) {
                        fds[j].fd = fds[j + 1].fd;
                    }
                    i--;
                    nfds--;
                }
            }
            std::cout << std::endl;
        }
    } /* End of serving running.    */

    void Server::printFds() {
        for (int i = 0; i < 10; ++i) {
            printf("fd: %d, ", fds[i].fd);
        }
    }



