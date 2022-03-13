#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <vector>
#include <cstdio>
#include <iostream>
#include <cstring>
#include "utils.hpp"
#include "Node.hpp"
#define BUFFSIZE 510

class Storage
{
private:
	Node				*data;

public:
	char 				buffer[BUFFSIZE];
	Storage();
	Storage(const Storage &other);
	Storage			&operator=(const Storage &other);
	~Storage();

	Node					 *getData() const;
	void 					setData();
	void 					clearBuffer();
	void					addNewNode();
	void 					printNodes();

};

#endif