#include "Node.hpp"

Node::Node(const std::string& content) : command(content), prev(NULL), next(NULL)
{

}

Node::Node(const Node &other)
{

}

Node &Node::operator=(const Node &other)
{
	return *this;
}

Node::~Node()
{
}


std::string Node::getCommand() const
{
	return command;
}
