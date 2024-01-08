#include "HTTPBody.hpp"
#include "StringTools.hpp"
#include <sstream>
#include <algorithm> 

HTTPBody::HTTPBody(void)
{
}

HTTPBody::~HTTPBody(void)
{
}

HTTPBody::HTTPBody(const HTTPBody &src)
{
	*this = src;
}

HTTPBody &HTTPBody::operator=(HTTPBody const &src)
{
	if (this != &src)
	{
		this->body = src.body;
	}
	return (*this);
}

void HTTPBody::addBodyChunk(const std::string & bodyChunk)
{
	body += bodyChunk;
}

std::string & HTTPBody::getFullBody(void)
{
	return body;
}
