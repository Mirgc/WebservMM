#include <sstream>

#include "HTTPResponse500.hpp"

HTTPResponse500::HTTPResponse500() {
	// This could be replaced by the configuration 500 error file set up if any.
    std::string bodyContent = "<!DOCTYPE html><html><body><h1>500 Internal error.</h1></body></html>";
	std::stringstream ss;

	ss << "HTTP/1.1 500\r\n"
		<< "Content-Type: text/html\r\n"
		<< "Content-Length: " << bodyContent.size()
		<< "\r\n\r\n"
		<< bodyContent;

	this->setResponse(ss.str());
}

