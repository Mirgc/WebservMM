#include <sstream>

#include "HTTPResponse405.hpp"

HTTPResponse405::HTTPResponse405() {
	// This could be replaced by the configuration 405 error file set up if any.
    std::string bodyContent = "<!DOCTYPE html><html><body><h1>405 Method Not Allowed.</h1></body></html>";
	std::stringstream ss;

	ss << "HTTP/1.1 405\r\n"
		<< "Content-Type: text/html\r\n"
		<< "Content-Length: " << bodyContent.size()
		<< "\r\n\r\n"
		<< bodyContent;

	this->setResponse(ss.str());
}

