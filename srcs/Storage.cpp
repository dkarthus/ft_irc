#include "Storage.hpp"

Storage::Storage()
{
	Storage::clearBuffer();
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


void Storage::setData()
{
	data += buffer;
	Storage::clearBuffer();
}

const std::string &Storage::getData() const
{
	return data;
}

void Storage::clearBuffer()
{
	for (int i = 0; i < BUFFSIZE; ++i)
	{
		buffer[i] = 0;
	}
}

