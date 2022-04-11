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

void Storage::addNewNode() {
	Node *new_node = new Node(std::string(this->buffer));
//	Node *temp;

//	temp = data;
//	if (temp)
//	{
//		while (temp->next != NULL)
//		{
//			temp = temp->next;
//		}
//		temp->next = new_node;
//		new_node->prev = temp;
//	}
//	else
//	{
//		data = new_node;
//	}
	new_node->next = data;
	data = new_node;
}

void Storage::setData()
{
	addNewNode();
	Storage::clearBuffer();
}

void Storage::printNodes() {
	Node		*temp;

	int count = 0;
	temp = data;
	while (temp){
		std::cout << "node " << count << " - data: " << temp->command << std::endl;
		temp = temp->next;
		count++;
	}
}

void Storage::clearBuffer()
{
	for (int i = 0; i < BUFFSIZE; ++i)
	{
		buffer[i] = 0;
	}
}

Node *Storage::getData() const
{
	return data;
}

