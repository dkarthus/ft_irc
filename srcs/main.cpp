#include "Server.hpp"
#include "Socket.hpp"

int			main(int argc, const char *argv[])
{
	if (argc == 3)
	{
		char* p;
		int converted = (int)strtol(argv[1], &p, 10);
		if (*p)
			std::cout << "Please indicate correct port" << std::endl;// conversion failed because the input wasn't a number
		else
		{
			Server			ircServer = Server(argv[1]);
			ircServer.pollConnections(ircServer.getServSocket().getListenSock());
		}
	}
	else {
		std::cout << "Wrong number of parameters" << std::endl;
		exit(1);
	}
	return (0);
}

