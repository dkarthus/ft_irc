#ifndef NODE_HPP
#define NODE_HPP

#include <unistd.h>
#include <string>
#include <iostream>

class Node
{
public:
	std::string 	command;
	Node			*prev;
	Node			*next;

public:
	Node(const std::string& content);
	Node(const Node &other);
	Node		&operator=(const Node &other);
	~Node();

	std::string getCommand() const;
};

#endif