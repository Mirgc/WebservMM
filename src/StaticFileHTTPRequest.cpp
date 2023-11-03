#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "StaticFileHTTPRequest.hpp"
#include "HTTPResponse.hpp"

StaticFileHTTPRequest::StaticFileHTTPRequest(): HTTPRequest() {
}

StaticFileHTTPRequest::StaticFileHTTPRequest(const StaticFileHTTPRequest & src)
    : HTTPRequest() {
        *this = src;
}

StaticFileHTTPRequest::~StaticFileHTTPRequest() {}

StaticFileHTTPRequest& StaticFileHTTPRequest::operator=(const StaticFileHTTPRequest &rhs) {
	if (this != &rhs) {

    }

	return (*this);
}

HTTPResponse StaticFileHTTPRequest::process() {
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your StaticFileHTTPRequest :) </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;

    HTTPResponse response;

    response.setResponse(ss.str());
    
    return (response);
}

