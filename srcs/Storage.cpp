#include "Storage.hpp"

Storage::Storage()
{
	Storage::clearBuffer();
	data = NULL;
}

Storage::Storage(const Storage &other)
{

}

Storage &Storage::operator=(const Storage &other)
{
	return (*this);
}

Storage::~Storage()
{

}


void Storage::setData(char *content)
{
	t_list 		*temp;

	temp = ft_lstnew(content);
	ft_lstadd_back(&data, temp);
	Storage::clearBuffer();
}


void Storage::clearBuffer()
{
	for (int i = 0; i < BUFFSIZE; ++i)
	{
		buffer[i] = 0;
	}
}

t_list *Storage::getData() const
{
	return data;
}

