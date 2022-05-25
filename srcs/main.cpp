#include "Server.hpp"
#include "Socket.hpp"

int			main(int argc, const char *argv[])
{
    int         rc;
    char        *p;

	if (argc == 3)
	{
		int converted = (int)strtol(argv[1], &p, 10);
		if (*p)
			std::cout << "Please indicate correct port" << std::endl;// conversion failed because the input wasn't a number
		else
		{
            Server			ircServer = Server(argv[1], argv[2]);
            for (;;)
            {
                printf("polling...\n");
                rc = poll(ircServer.fds_vec.data(), ircServer.fds_vec.size(), ircServer.timeout);
                if (rc <= 0) {
                    perror("  poll() failed or timed out.  End program.\n");
                    return (1);
                }
//                ircServer.currentSize = ircServer.nfds;
                ircServer.pollConnections(ircServer.getServSocket().getListenSock());
//                if ((ircServer.pollConnections(ircServer.getServSocket().getListenSock())) == 1)
//                {
//                        int nfds = ircServer.currentSize;
//                        for (int i = 0; i < nfds; i++) {
//                            if (ircServer.fds_vec[i].fd == -1) {
//                                for (int j = i; j < nfds; j++) {
//                                    ircServer.fds_vec[j].fd = ircServer.fds_vec[j + 1].fd;
//                                }
//                                i--;
//                                nfds--;
//                            }
//                        }
//                        std::cout << std::endl;
//                        //}
//                } /* End of serving running.    */
            }
        }
    }
	else
    {
		std::cout << "Wrong number of parameters" << std::endl;
        return (1);
	}
	return (0);
}

