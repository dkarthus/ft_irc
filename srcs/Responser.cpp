#include "Responser.hpp"
#include <sstream>
#include <iostream>

Responser::Responser()
{
}

Responser::Responser(const Responser &other)
{

}

Responser &Responser::operator=(const Responser &other)
{
	return *this;
}

Responser::~Responser()
{

}

void Responser::sendResponse(int fd, int respCode, std::string &nick)
{
	std::stringstream	ss;
	ss << respCode;
	std::string 		serverName = "!Adium@127.0.0.1";
//	std::string 		nick;
	std::string 		response = ":" + serverName + " " + ss.str() + " " + nick + " :";
	switch (respCode)
	{
		case RPL_MOTDSTART:
			response = response + "- IRCSERV Message of the day - \n";
			break;
		case RPL_MOTD:
			response = response + "- IRC Welcome to server!!!\n";
			break;
		case RPL_ENDOFMOTD:
			response = response + "End of /MOTD command\n";
			break;
		case RPL_AWAY:
			response = response + nick + "  : the user is away\n";
	}
	std::cout << "TO fd [" << fd <<  "] send a message '" << response << "'" << std::endl;
	send(fd, response.c_str(), response.length(), 0);
}

void 			Responser::sendMotd(int fd, std::string &nick)
{
	this->sendResponse(fd, RPL_MOTDSTART, nick);
	this->sendResponse(fd, RPL_MOTD, nick);
	this->sendResponse(fd, RPL_ENDOFMOTD, nick);
}

int Responser::sendError(int fd, int errorCode, const std::string& command)
{
	std::string 		error;
	std::string 		serverName = "!Adium@127.0.0.1";
	std::stringstream	ss;
	std::string 		nick = "";
	std::string 		channelName;
	ss << errorCode;

	error = ':' + serverName + " " + ss.str();
	switch (errorCode)
	{
		case ERR_NEEDMOREPARAMS:
			error = error + command + " :Not enough parameters\n";
			break;
		case ERR_NONICKNAMEGIVEN:
			error = error + ":No nickname given\n";
			break;
		case ERR_NICKNAMEINUSE:
			error = error + nick + " :Nickname is already in use\n";
			break;
		case ERR_ERRONEUSNICKNAME:
			error = error + nick + " :Erroneus nickname\n";
			break;
		case ERR_NICKCOLLISION:
			error = error + nick + " :Nickname collision KILL\n";
			break;
		case ERR_ALREADYREGISTRED:
			error = error + ":You may not reregister\n";
			break;
		case ERR_NORECIPIENT:
			error = error + ":No recipient given " + command + "\n";
			break;
		case ERR_CANNOTSENDTOCHAN:
			error = error + channelName + "  :Cannot send to channel\n";
			break;
		case ERR_NOSUCHNICK:
			error = error + nick + " No such nick/channel\n";
			break;
		case ERR_NOTEXTTOSEND:
			error = error + " :No text to send\n";
			break;
        case ERR_NOSUCHCHANNEL:
            error = error + " :No such channel\n";
            break;
        case ERR_BADCHANNELKEY:
            error = error + ":Cannot join channel (+k)\n";
            break;
		case ERR_CHANOPRIVSNEEDED:
			error += " " + command + " :You're not channel operator\n";
			break;
		case ERR_NOTONCHANNEL:
			error += " " + command + " :You're not on that channel\n";
			break;
		case ERR_USERNOTINCHANNEL:
			error += " " + command + " :They aren't on that channel\n";
			break;
		case ERR_USERONCHANNEL:
			error += " " + command + " :is already on channel\n";
			break;
		case ERR_UMODEUNKNOWNFLAG:
			error += " :Unknown MODE flag\n";
			break;
		case ERR_KEYSET:
			error += " " + command + " :Channel key already set\n";
			break;
		case ERR_UNKNOWNMODE:
			error += " " + command + " :is unknown mode char to me\n";
			break;
		case ERR_USERSDONTMATCH:
			error += " :Cant change mode for other users\n";
			break;
	}
	std::cout << "Printing error" << error << std::endl;
	send(fd, error.c_str(), error.length(), 0);
    return(-1);
}

int Responser::sendAnswerJoin(int fd, int errorCode, const std::string& nick, const std::string& name)
{
    std::string 		error;
    std::string 		serverName = "!Adium@127.0.0.1";
    std::stringstream	ss;
    std::string 		nick_op;
    std::string 		channelName;
    ss << errorCode;

    nick_op = nick;
    if(nick[0] == '@')
        nick_op = nick_op.erase(0,1);

    error = ':' + serverName + " " + ss.str() + " " ;
    switch (errorCode)
    {
        case RPL_NOTOPIC:
            error = error + nick + " " + name + " :No topic is set\n";
            break;
//        case RPL_NAMREPLY:
//            error = error + nick_op + " = " + name + " :" + nick +"\n";
//            break;
//        case RPL_NAMREPLY1:
//            error = error + nick + " = " + name + " :" +nick+"\n";
//            break;
        case RPL_ENDOFNAMES:
            error = error + nick + " " + name + " :End of /NAMES list\n";
            break;
    }
    std::cout << "Printing AnswerJoin" << error << std::endl;
    send(fd, error.c_str(), error.length(), 0);
    return(-1);
}

int Responser::sendListUsers(int fd, int errorCode, const std::string& nick, const std::string& nicksUser, const std::string& name)
{
    std::string 		error;
    std::string 		serverName = "!Adium@127.0.0.1";
    std::stringstream	ss;
//    std::string 		nick_op;
    std::string 		channelName;
    ss << errorCode;

//    nick_op = nick;
//    if(nick[0] == '@')
//        nick_op = nick_op.erase(0,1);

    error = ':' + serverName + " " + ss.str() + " " ;
    switch (errorCode)
    {
        case RPL_NAMREPLY:
            error = error + nick + " = " + name + " :" + nicksUser +"\n";
            break;
    }
    std::cout << "Printing AnswerJoin" << error << std::endl;
    send(fd, error.c_str(), error.length(), 0);
    return(-1);
}