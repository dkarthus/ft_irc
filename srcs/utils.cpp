#include "Server.hpp"
#include "Storage.hpp"

t_list	*ft_lstnew(void *content)
{
    t_list	*elt;

    if (!(elt = (t_list*)malloc(sizeof(*elt))))
        return (NULL);
    elt->content = content;
    elt->prev = NULL;
    elt->next = NULL;
    return (elt);
}