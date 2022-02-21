#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cerrno>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <string>
//#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>
#include <fcntl.h>

typedef	struct	s_list
{
	char 			*command;
	struct s_list	*prev;
	struct s_list	*next;
}				t_list;

t_list 				*ft_lstnew(char *content);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **alst, t_list *newList);
void				ft_lst_print(t_list *lst);
char 				*ft_strdup(const char *s1);
size_t				ft_strlen(const char *s);

#endif