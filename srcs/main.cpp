#include "Server.hpp"
#include "Socket.hpp"

int			main(int argc, char *argv[])
{
	int						listen_sd, new_sd;
	struct addrinfo			hints, *res, *tmp;
	int 					rv, rc;
	int 					yes = 1;
	struct pollfd 			fds[200]; // need dynamic allocation
	int 					timeout;
	int    					nfds = 1, current_size = 0, i, j;
	int    					end_server = FALSE, compress_array = FALSE;
	int    					close_conn;
	char   					buffer[80];
	int 					len;

	Socket  				servSocket;
	Server					ircServer;

	servSocket = Socket();
	ircServer = Server();
	servSocket.createAddrinfo();
	ircServer.startSocket(servSocket);
	ircServer.listenConnections(servSocket.getListenSock());
	ircServer.initFdStruct(servSocket.getListenSock());
	ircServer.pollConnections(servSocket.getListenSock());


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

