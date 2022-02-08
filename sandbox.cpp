#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#define MYPORT "3490"
#define BACKLOG 10

void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}
// получить sockaddr, IPv4 или IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *) sa)->sin_addr);
	return &((struct sockaddr_in6 *)sa)->sin6_addr;
}

int			main(int argc, char *argv[])
{
	int 								status, sockfd, new_fd;
	struct sockaddr_storage				their_addr;
	socklen_t 							sin_size;
	struct sigaction					sa;
	int 								yes = 1;
	char 								s[INET6_ADDRSTRLEN];
	fd_set 								master;
	fd_set 								read_fds;
	int 								fd_max;
	int 								listener;
	int 								newfd;
	struct sockaddr_storage				remoteaddr;
	socklen_t 							addrlen;
	char								buf[256];
	int									nbytes;
	char								remoteIP[INET6_ADDRSTRLEN];
	int 								i, j, rv;
	struct addrinfo						hints, *ai, *p;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &ai)) != 0)
	{
		fprintf(stderr, "select server: %s\n", gai_strerror(rv));
		exit(1);
	}
	for (p = ai; p != NULL; p = p->ai_next)
	{
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0)
			continue;
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int));
		if ((bind(listener, p->ai_addr, p->ai_addrlen)) < 0)
		{
			close(listener);
			perror("server: bind");
			continue;
		}
		break;
	}
	if (p == NULL)
	{
		fprintf(stderr, "selectserver: failed to bind");
		exit(2);
	}
	freeaddrinfo(ai); // why do we free here?
	if (listen(listener, BACKLOG) == -1)
	{
		perror("listen");
		exit(3);
	}
	FD_SET(listener, &master);
	fd_max = listener;
	printf("server waiting for connections...\n");
	for (;;)
	{
		read_fds = master;
		if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit(4);
		}
		for (i = 0; i <= fd_max ; i++)
		{
			if (FD_ISSET(i, &read_fds))
			{
				if (i == listener)
				{
					addrlen = sizeof(remoteaddr);
					new_fd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);
					if (new_fd == -1)
						perror("accept");
					else
					{
						FD_SET(new_fd, &master);
						if ()
					}
				}

			}
		}

	}
	return (0);
}
