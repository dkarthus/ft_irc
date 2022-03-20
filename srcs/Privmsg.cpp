#include "Privmsg.hpp"

void 		sendPrivmsg(int fd, char *message) {
	std::cout << "send msg '" << message << "' to fd " << fd << std::endl;
	send(fd, message, sizeof(message), 0);
}