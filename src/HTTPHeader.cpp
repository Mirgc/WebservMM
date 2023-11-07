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
	std::cout << this->header[i].first << " " << this->header[i].second; // HTTP status
	for (size_t i = 1; i< this->header.size(); ++i) {
		std::cout << this->header[i].first << ": " << this->header[i].second;
		std::cout << std::endl;
	}
}

std::string	HTTPHeader::generateResponse(void) const {
	std::string	response;

	response += this->header[i].first + " " + this->header[i].second + "\n"; //HTTP status
	for (size_t i = 1; i< this->header.size(); ++i) {
		response += this->header[i].first + ": " + this->header[i].second + "\n";
	}
	return response;
}
