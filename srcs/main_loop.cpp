#include "Server.hpp"
#include "Socket.hpp"


void	sigHandler(int signum)
{
	(void)signum;
	killswitch = false;
}

int			main(int argc, char *argv[])
{
	struct pollfd 			fds[200]; // need dynamic allocation
	int    					nfds = 1, i;

	Socket  				servSocket;
	Server					ircServer;

	servSocket = Socket();
	ircServer = Server();
	servSocket.createAddrinfo();
	ircServer.startSocket(servSocket);
    
    signal(SIGINT, sigHandler);
    
    while (killswitch)
    {
	ircServer.listenConnections(servSocket.getListenSock());
	ircServer.initFdStruct(servSocket.getListenSock());
	ircServer.pollConnections(servSocket.getListenSock());
    }

	/*************************************************************/
	/* Clean up all of the sockets that are open                 */
	/*************************************************************/
	for (i = 0; i < nfds; i++)
	{
		if(fds[i].fd >= 10)
			close(fds[i].fd);
	}
	return (0);
}

