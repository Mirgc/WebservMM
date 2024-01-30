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

const std::string & HTTPBody::getFullBody(void) const
{
	return body;
}

std::size_t HTTPBody::getBodySize(void) const
{
	return body.size();
}

std::string HTTPBody::getBodySizeString(void) const
{
    std::stringstream ss;
    ss << this->getBodySize();
    return ss.str();
}
