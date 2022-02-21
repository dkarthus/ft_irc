#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <vector>
#include <cstdio>
#include <iostream>
#include <cstring>
#define BUFFSIZE 15

class Storage
{
private:
	std::string 		data;
	typedef	struct	s_list
	{
	    std::string 	*command;
	    struct s_list	*prev;
	    struct s_list	*next;
	}				t_list;
public:
    Storage();
    char 				buffer[BUFFSIZE];
	Storage(const Storage &other);
	Storage			&operator=(const Storage &other);
	~Storage();
	void 						setData();

	const std::string 			&getData() const;
	void clearBuffer();
};

#endif