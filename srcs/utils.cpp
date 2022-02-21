#include "Server.hpp"
#include "Storage.hpp"

size_t		ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char 		*ft_strdup(const char *s1)
{
	char	*str;
	size_t	i;

	if (!s1)
		return (NULL);
	str = (char*)malloc(sizeof(*s1) * (ft_strlen(s1) + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

t_list		*ft_lstlast(t_list *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

t_list 		*ft_lstnew(char *content)
{
	t_list	*elt;

	if (!(elt = (t_list*)malloc(sizeof(*elt))))
		return (NULL);
	elt->command = ft_strdup(content);
	elt->prev = NULL;
	elt->next = NULL;
	return (elt);
}

void		ft_lstadd_back(t_list **alst, t_list *newNode)
{
	t_list	*last;

	if (alst)
	{
		if (*alst)
		{
			last = ft_lstlast(*alst);
			last->next = newNode;
		}
		else
			*alst = newNode;
	}
}

void		ft_lst_print(t_list *lst)
{
	t_list 		*temp;                   //Временный указатель на адрес последнего элемента

	//ВЫВОДИМ СПИСОК С НАЧАЛА
	temp = lst;                       //Временно указываем на адрес первого элемента
	while (temp != NULL)              //Пока не встретим пустое значение
	{
		std::cout << temp->command << " ";        //Выводим каждое считанное значение на экран
		temp = temp->next;             //Смена адреса на адрес следующего элемента
	}
	std::cout << "\n";
}