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
#include <sys/time.h>
#include <signal.h>
#define MYPORT "3490"
#define BACKLOG 10
#define STDIN 0

int		main()
{
	struct timeval		tv;
	fd_set 				readfds;

	tv.tv_sec = 2;
	tv.tv_usec = 500000;

	FD_ZERO(&readfds);
	FD_SET(STDIN, &readfds);

	select(STDIN + 1, &readfds, NULL, NULL, &tv);
	if (FD_ISSET(STDIN, &readfds))
		printf("A key was pressed!\n");
	else
		printf("timed out\n");
	return (0);
}