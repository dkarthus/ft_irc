
#include "Channel.hpp"

Channel::Channel(const std::string &name, const User *creator, const std::string &pass) :
		name(name), pass(pass), flags("n")
{

	users.push_back(creator);
	ops.push_back(creator);
    sendMessageJoin(creator, name, "JOIN");
    responser.sendAnswerJoin(creator->getSockfd(), RPL_NOTOPIC, creator->getNickname(), name);
    answerOper(creator);
    responser.sendAnswerJoin(creator->getSockfd(), RPL_ENDOFNAMES, creator->getNickname(), name);

}

void Channel::answerOper(const User *user){
    std::string	names;
    std::vector<const User *>::const_iterator	beg = users.begin();
    std::vector<const User *>::const_iterator	end = users.end();
    while (beg != end) {
        const User *tmp = *beg;
        for (size_t i = 0; i < ops.size(); i++)
            if (ops[i]->getNickname() == (tmp)->getNickname())
                names += "@";
        names += tmp->getNickname();
        ++beg;
        if (beg != end)
            names += " ";
    }
    responser.sendListUsers(user->getSockfd(), RPL_NAMREPLY, user->getNickname(), names,  name);

}

void Channel::CheckConnect(const User *user, const std::string &passU){
    if (passU != pass)
        responser.sendError(user->getSockfd(), ERR_BADCHANNELKEY, "JOIN");
    else{
        users.push_back(user);
        sendMessageJoin(user, getName(), "JOIN");
        std::string mssg=":IRCat 315 kalexand kalexand :End of /WHO list\n";
        send(5, mssg.c_str(), mssg.size(), IRC_NOSIGNAL);

        responser.sendAnswerJoin(user->getSockfd(), RPL_NOTOPIC, user->getNickname(), getName());
        answerOper(user);
        responser.sendAnswerJoin(user->getSockfd(), RPL_ENDOFNAMES, user->getNickname(), name);

    }

}
void Channel::sendMessageJoin(const User *user, const std::string	name, std::string command){
    std::string	msg;
    msg += ":" + user->getNickname() + "!Adium@127.0.0.1 " + command + " :" + name+ "\n";
    std::vector<const User *>::const_iterator	begin = users.begin();
    std::vector<const User *>::const_iterator	end = users.end();
    for (; begin != end; ++begin)
    {
            (*begin)->sendMessage(msg);
    }

};

void Channel::sendMessagePrivmsg(const User *user, const std::string	name, std::string command, std::string mess){
    std::string	msg;
    msg += ":" + user->getNickname() + "!Adium@127.0.0.1 " + command  + " " +  name + " :" + mess + "\n";
    std::vector<const User *>::const_iterator	begin = users.begin();
    std::vector<const User *>::const_iterator	end = users.end();
    for (; begin != end; ++begin)
    {
        (*begin)->sendMessage(msg);
    }

}

Channel::~Channel()
{}
//
//
const std::string	&Channel::getName() const
{
	return (name);
}

//
//const std::string	&Channel::getTopic() const
//{
//	return (topic);
//}
//
//void	Channel::setTopic(const User &user, const std::string &topic)
//{
//	if ((flags.find("t")) && !isOperator(user))
//		//SEND ERROR unable to set topic
//		return;
//	else
//	{
//		this->topic = topic;
//		// send MSG TOPIC SET
//	}
//}
//
//bool	Channel::isOperator(const User &user) const
//{
//	for (size_t i = 0; i < ops.size(); i++)
//		if (ops[i]->getNickname() == user.getNickname())
//			return true;
//	return false;
//}
//
//void	Channel::setPass(const User &user, const std::string &newPass)
//{
//	if (pass.size() > 0 && newPass.size() > 0)
//		//SEND MSG ERROR KEY SET
//		return;
//	else
//		this->pass = newPass;
//}
//
//const std::string	&Channel::getFlags() const
//{
//	return flags;
//}
//
//bool	Channel::isBanned(const User &user) const
//{
//	for (size_t i = 0; i < banned.size(); i++)
//		if (banned[i] == user.getNickname())
//			return true;
//	return false;
//}
//
//bool	Channel::isEmpty() const
//{
//	if (users.size() == 0)
//		return true;
//	return false;
//}
//
//void	Channel::setFlag(std::string &flag)
//{
//	std::string::size_type n;
//	if ((n = flags.find(flag)) != std::string::npos)
//		flags.erase(n);
//	else
//		flags += flag;
//}
//
//void	Channel::removeFlag(std::string &flag)
//{
//	std::string::size_type n;
//	if ((n = flags.find(flag)) == std::string::npos)
//		flags.erase(n);
//}
//
//
//void	Channel::addOperator(const User &user)
//{
//	if (!isOperator(user))
//		ops.push_back(&user);
//}
//
//void	Channel::removeOperator(const User &user)
//{
//	if (isOperator(user))
//	{
//		size_t	i;
//		for (i = 0; i < ops.size(); i++)
//			if (ops[i] == &user)
//				break;
//		ops.erase(ops.begin() + i);
//		if (ops.size() == 0 && users.size() > 0)
//		{
//			ops.push_back(users[0]);
//			//SEND MSG ABOUT NEW OPS?
//		}
//	}
//}
//
//
//void	Channel::addBanned(const std::string &nick)
//{
//	banned.push_back(nick);
//}
//
//void	Channel::removeBanned(const std::string &nick)
//{
//	size_t	i = 0;
//	while (i < banned.size())
//	{
//		if (banned[i] == nick)
//		{
//			banned.erase(banned.begin() + i);
//			break ;
//		}
//		++i;
//	}
//}
//
//void	Channel::disconnect(const User &user)
//{
//	std::vector<const User *>::iterator	begin = users.begin();
//	std::vector<const User *>::iterator	end = users.end();
//	for (; begin != end; ++begin)
//		if (*begin == &user)
//			break ;
//	users.erase(begin);
//	removeOperator(user);
//
//}
