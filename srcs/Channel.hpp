//
// Created by Drahff Karthus on 4/17/22.
//

#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Responser.hpp"
#include <string>
#include <vector>
#include "User.hpp"
#include "Privmsg.hpp"
class User;

class Channel
{
public:
    Responser responser;
	Channel(const std::string &name, const User *creator, const std::string &pass = "");
	virtual ~Channel();
    void CheckConnect(const User *user, const std::string &pass);
    void sendMessageJoin(const User *user, const std::string	name, std::string command);
    void answerOper(const User *user);
    void sendMessagePrivmsg(const User *user, const std::string	name, std::string command, std::string mess);
    void sendMessageKick(const User *user, const std::string	name, std::string command);
	void sendMessageInvite(const User *user, int replay, const std::string	from, const std::string chName, const
	std::string nick);
	void	sendMessageMode(const std::string &message, const User &op) const;

//
	const std::string				&getName() const;
//	const std::string				&getTopic() const;
	const std::string				&getFlags() const;
//
//	void							setTopic(const User &user, const std::string &topic);
	void							setLimit(unsigned short limit);
	void							setPass(const User &user, const std::string &newPass);
	void							setFlag(const std::string &flag);
	void							removeFlag(const std::string &flag);
	int								processFlags(const Message &msg, User &op, User &rec);
	bool 							containsFlag(const std::string &flag) const;
//
	bool							isOperator(const User &user) const;
	bool							isBanned(const User &user) const;
	bool							isMember(const std::string &nick) const;
//	bool							isEmpty() const;

	void							addUser(const User &user);
	void							removeUser(const User &user);
	void							addBanned(const std::string &nick);
	void							removeBanned(const std::string &nick);
//	void							invite(const User &user, const User &receiver);
	void							addOp(const User &user);
	void							removeOp(const User &user);
    bool	                        isUser(std::string name) const;
//	void							displayTopic(const User &user);
//	void							disconnect(const User &user);

private:
	std::string						name;
	std::vector<const User *>		ops;
	std::vector<const User *>		users;
	std::vector<std::string>		banned;
	std::string						pass;
//	std::string						topic;
	std::string						flags;
};

#endif //FT_IRC123_CHANNEL_HPP
