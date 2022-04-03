#include "Privmsg.hpp"

void 		sendPrivmsg(int fd, char *message) {
	std::string 		sender = "aizhan";
	std::string 		receiver = "azat";
	std::string 		myMessage;

	std::cout << "SEND MSG'" << message << "' to fd " << fd << std::endl;
	myMessage = ":" + receiver + "!oem@127.0.0.1" + " PRIVMSG " + sender + " :Hello";
	printf("printing myMessage: %s\n", myMessage.c_str());
	send(fd + 1, myMessage.c_str(), sizeof(myMessage.c_str()), 0);
}