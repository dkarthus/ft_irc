//
// Created by ulistonee on 08.02.2022.
//
#include "Server.hpp"

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

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	// need to address port receival
	if ((rv = getaddrinfo(NULL, SERVER_PORT, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo server: %s\n", gai_strerror(rv));
		exit(1);
	}
	for (tmp = res; tmp != NULL; tmp->ai_next)
	{
		/*************************************************************/
		/* Create an AF_INET6 stream socket to receive incoming      */
		/* connections on                                            */
		/*************************************************************/
		listen_sd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
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
	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/
	rc = listen(listen_sd, 10);
	if (rc < 0)
	{
		perror("listen() failed");
		close(listen_sd);
		exit(1);
	}
	/*************************************************************/
	/* Initialize the pollfd structure                           */
	/*************************************************************/
	memset(fds, 0, sizeof(fds));
	/*************************************************************/
	/* Set up the initial listening socket                        */
	/*************************************************************/
	fds[0].fd = listen_sd;
	fds[0].events = POLLIN;
	/*************************************************************/
	/* Initialize the timeout to 3 minutes. If no                */
	/* activity after 3 minutes this program will end.           */
	/* timeout value is based on milliseconds.                   */
	/*************************************************************/
	timeout = (30 * 60 * 1000);
	/*************************************************************/
	/* Loop waiting for incoming connects or for incoming data   */
	/* on any of the connected sockets.                          */
	/*************************************************************/
	do
	{
		printf("Waiting on poll()...\n");
		/***********************************************************/
		/* Call poll() and wait 3 minutes for it to complete.      */
		/***********************************************************/
		rc = poll(fds, nfds, timeout);
		/***********************************************************/
		/* Check to see if the poll call failed.                   */
		/***********************************************************/
		if (rc < 0)
		{
			perror("  poll() failed");
			break;
		}
		/***********************************************************/
		/* Check to see if the 3 minute time out expired.          */
		/***********************************************************/
		if (rc == 0)
		{
			printf("  poll() timed out.  End program.\n");
			break;
		}
		current_size = nfds;
		for (i = 0; i < current_size; i++)
		{
			/*********************************************************/
			/* Loop through to find the descriptors that returned    */
			/* POLLIN and determine whether it's the listening       */
			/* or the active connection.                             */
			/*********************************************************/
			if (fds[i].revents == 0)
				continue;
			/*********************************************************/
			/* If revents is not POLLIN, it's an unexpected result,  */
			/* log and end the server.                               */
			/*********************************************************/
			if (fds[i].revents != POLLIN)
			{
				printf("  Error! revents = %d\n", fds[i].revents);
				end_server = TRUE;
				break;
			}
			if (fds[i].fd == listen_sd)
			{
				/*******************************************************/
				/* Listening descriptor is readable.                   */
				/*******************************************************/
				printf("  Listening socket is readable\n");
				/*******************************************************/
				/* Accept all incoming connections that are            */
				/* queued up on the listening socket before we         */
				/* loop back and call poll again.                      */
				/*******************************************************/
				do
				{
					/*****************************************************/
					/* Accept each incoming connection. If               */
					/* accept fails with EWOULDBLOCK, then we            */
					/* have accepted all of them. Any other              */
					/* failure on accept will cause us to end the        */
					/* server.                                           */
					/*****************************************************/
					new_sd = accept(listen_sd, NULL, NULL);
					if (new_sd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("  accept() failed");
							end_server = TRUE;
						}
						break;
					}
					/*****************************************************/
					/* Add the new incoming connection to the            */
					/* pollfd structure                                  */
					/*****************************************************/
					printf("  New incoming connection - %d\n", new_sd);
					fds[nfds].fd = new_sd;
					fds[nfds].events = POLLIN;
					nfds++;
					/*****************************************************/
					/* Loop back up and accept another incoming          */
					/* connection                                        */
					/*****************************************************/
				}
				while (new_sd != -1);
			}
				/*********************************************************/
				/* This is not the listening socket, therefore an        */
				/* existing connection must be readable                  */
				/*********************************************************/
			else
			{
				printf("  Descriptor %d is readable\n", fds[i].fd);
				close_conn = FALSE;
				/*******************************************************/
				/* Receive all incoming data on this socket            */
				/* before we loop back and call poll again.            */
				/*******************************************************/

				do
				{
					/*****************************************************/
					/* Receive data on this connection until the         */
					/* recv fails with EWOULDBLOCK. If any other         */
					/* failure occurs, we will close the                 */
					/* connection.                                       */
					/*****************************************************/
					rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					if (rc < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("  recv() failed");
							close_conn = TRUE;
						}
						break;
					}

					/*****************************************************/
					/* Check to see if the connection has been           */
					/* closed by the client                              */
					/*****************************************************/
					if (rc == 0)
					{
						printf("  Connection closed\n");
						close_conn = TRUE;
						break;
					}

					/*****************************************************/
					/* Data was received                                 */
					/*****************************************************/
					len = rc;
					printf("  %d bytes received\n", len);

					/*****************************************************/
					/* Echo the data back to the client                  */
					/*****************************************************/
					rc = send(fds[i].fd, buffer, len, 0);
					if (rc < 0)
					{
						perror("  send() failed");
						close_conn = TRUE;
						break;
					}

				} while(TRUE);

				/*******************************************************/
				/* If the close_conn flag was turned on, we need       */
				/* to clean up this active connection. This            */
				/* clean up process includes removing the              */
				/* descriptor.                                         */
				/*******************************************************/
				if (close_conn)
				{
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_array = TRUE;
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
		if (compress_array)
		{
			compress_array = FALSE;
			for (i = 0; i < nfds; i++)
			{
				if (fds[i].fd == -1)
				{
					for(j = i; j < nfds; j++)
					{
						fds[j].fd = fds[j+1].fd;
					}
					i--;
					nfds--;
				}
			}
		}

	} while (end_server == FALSE); /* End of serving running.    */

	/*************************************************************/
	/* Clean up all of the sockets that are open                 */
	/*************************************************************/
	for (i = 0; i < nfds; i++)
	{
		if(fds[i].fd >= 0)
			close(fds[i].fd);
	}
	return (0);
}

