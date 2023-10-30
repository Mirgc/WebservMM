#include "HTTPHeader.hpp"

HTTPHeader::HTTPHeader(void) {
}

HTTPHeader::~HTTPHeader(void) {
}

HTTPHeader::HTTPHeader(const HTTPHeader & src) {
	*this = src;
}

HTTPHeader & HTTPHeader::operator = (HTTPHeader const & src) {
	if (this != &src) {
		this->header = src.header;
	}
	return (*this);
}

//Could be null values?? if so, we can check value1 & value2  anf throw exception
void	HTTPHeader::addHeader(const std::string & value1, const std::string & value2) {
	this->header.push_back(std::make_pair(value1, value2));
}

void	HTTPHeader::printHeader(void) const {
	for (size_t i = 0; i< this->header.size(); ++i) {
		std::cout << this->header[i].first << " " << this->header[i].second;
		std::cout << std::endl;
	}
}

//Be careful if there is not ':' in the first string of each position
std::string	HTTPHeader::generateResponse(void) const {
	std::string	response;

	for (size_t i = 0; i< this->header.size(); ++i) {
		response += this->header[i].first + " " + this->header[i].second + "\n";
	}
	return response;
}
