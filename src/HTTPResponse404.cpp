#include <sstream>

#include "HTTPResponse404.hpp"

HTTPResponse404::HTTPResponse404() {
	// This could be replaced by the configuration 404 error file set up if any.
    std::string bodyContent = "<!DOCTYPE html><html><body><h1>404 Not Found.</h1></body></html>";
	std::stringstream ss;

	ss << "HTTP/1.1 404\r\n"
		<< "Content-Type: text/html\r\n"
		<< "Content-Length: " << bodyContent.size()
		<< "\r\n\r\n"
		<< bodyContent;

	this->setResponse(ss.str());
}

