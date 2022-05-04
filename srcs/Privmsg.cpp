//#include "Privmsg.hpp"
#include "Server.hpp"

int 	Server::sendPrivmsg(int fd, const std::vector<std::string> param, int fd_send, std::string sender, User *user) {
	std::string 		receiver;
	std::string 		myMessage;
    Responser response;

    receiver = param[0];

    if (receiver[0] == '#' && fd_send == -1){
        Channel *receiverChannel = this->channels[receiver];
        receiverChannel->sendMessageJoin(user, param[1]);
    }
    else {
//	std::cout << "SEND MSG'" << param[1] << "' to fd " << fd_send<< std::endl;
//    :kalexand!Adium@127.0.0.1 PRIVMSG yuliya :k
        myMessage = ":" + sender + "!Adium@127.0.0.1" + " PRIVMSG " + receiver + " :" + param[1] + "\n";
//	printf("printing myMessage: %s\n", myMessage.c_str());
//    std::string send_mess = param[1] + "\n";
//    std::cout<< myMessage.size()<< "\n"<< sizeof(myMessage.c_str())<< std::endl;
        send(fd_send, myMessage.c_str(), myMessage.size(), 0);
    }
    return(0);
}


int sendMessageJoin(User *user, const std::string &name){

}