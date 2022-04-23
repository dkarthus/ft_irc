//
// Created by Drahff Karthus on 4/17/22.
//

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "User.hpp"

class User;

class Channel
{
public:
	Channel(const std::string &name, const User &creator, const std::string &pass = "");
	virtual ~Channel();

	const std::string				&getName() const;
	const std::string				&getTopic() const;
	const std::string				&getFlags() const;

	void							setTopic(const User &user, const std::string &topic);
	void							setLimit(unsigned short limit);
	void							setPass(const User &user, const std::string &newPass);
	void							setFlag(std::string &flag);
	void							removeFlag(std::string &flag);

	bool							isOperator(const User &user) const;
	bool							isBanned(const User &user) const;
	bool							isEmpty() const;

	void							addBanned(const std::string &nick);
	void							removeBanned(const std::string &nick);
	void							invite(const User &user, const User &receiver);
	void							addOperator(const User &user);
	void							removeOperator(const User &user);
	void							displayTopic(const User &user);
	void							disconnect(const User &user);

private:
	std::string						name;
	std::vector<const User *>		ops;
	std::vector<const User *>		users;
	std::vector<std::string>	banned;
	std::string						pass;
	std::string						topic;
	std::string						flags;
};

#endif //FT_IRC123_CHANNEL_HPP
