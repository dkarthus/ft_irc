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
	struct addrinfo 					hints, *res, *p;
	struct sockaddr_storage				their_addr;
	socklen_t 							sin_size;
	struct sigaction					sa;
	int yes = 1;
	char 		s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(NULL, MYPORT, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(2);
	}
	for (p = res; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
					   sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
		if ((bind(sockfd, p->ai_addr, p->ai_addrlen)) == -1)
		{
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	if (p == NULL)
	{
		fprintf(stderr, "server: failed to bind");
		return (2);
	}
	freeaddrinfo(res); // why do we free here?
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
	printf("server waiting for connections...\n");
	while (1)
	{
		sin_size = sizeof(their_addr);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
		{
			printf("new_fd: %d\n", new_fd);
			printf("sockfd: %d\n", sockfd);
			perror("server: accept");
			break;
		}
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s,
													sizeof(s));
		printf("server: got connection from %s\n", s);
		if (!fork())
		{
			close(sockfd);
			if (send(new_fd, "Aizhan was here", 15, 0) == -1)
			{
				perror("send");
			}
			close(new_fd);
			exit(0);
		}
	}
	return (0);
}
