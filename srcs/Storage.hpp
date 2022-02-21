#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <vector>
#include <cstdio>
#include <iostream>
#include <cstring>
#include "utils.hpp"
#define BUFFSIZE 15

class Storage
{
private:
//	std::string 		data;
	t_list 				*data;

public:

	char 				buffer[BUFFSIZE];
	Storage();
	Storage(const Storage &other);
	Storage			&operator=(const Storage &other);
	~Storage();

	t_list					 *getData() const;
	void 					setData(char *content);
	void 					clearBuffer();

};

#endif