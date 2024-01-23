#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "ErrorHTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTTPResponse404.hpp"
#include "HTTPResponse405.hpp"
#include "HTTPResponse500.hpp"

ErrorHTTPRequest::ErrorHTTPRequest(
    const ServerConfig & serverConfig,
    const LocationConfig & location,
    const HTTPHeader & httpHeader,
    const int httpResponseStatusCode
): HTTPRequest(serverConfig, location, httpHeader) {
    this->httpResponseStatusCode = httpResponseStatusCode;
}

ErrorHTTPRequest::ErrorHTTPRequest(const ErrorHTTPRequest & src)
    : HTTPRequest(src.serverConfig, src.location, src.httpHeader) {
        *this = src;
}

ErrorHTTPRequest::~ErrorHTTPRequest() {}

ErrorHTTPRequest& ErrorHTTPRequest::operator=(const ErrorHTTPRequest &rhs) {
	if (this != &rhs) {
        this->httpResponseStatusCode = rhs.httpResponseStatusCode;
    }
	return (*this);
}

// We need a way to clone a derived instance not knowing its derived form
ErrorHTTPRequest * ErrorHTTPRequest::clone() {
    return (new ErrorHTTPRequest(*this));
}

HTTPResponse ErrorHTTPRequest::process() {
    try {
        if (this->httpResponseStatusCode >= 400 && this->httpResponseStatusCode < 500) {
            if (this->httpResponseStatusCode == 405) {
                return HTTPResponse405();
            }
            return HTTPResponse404();
        }
        else if (this->httpResponseStatusCode >= 500 && this->httpResponseStatusCode < 600) {
            return HTTPResponse500();
        }

        std::cout << "ErrorHTTPRequest: Not supportted http response status code: " << this->httpResponseStatusCode << std::endl;

    } catch (...) {
        return HTTPResponse500();
    }

    return (HTTPResponse500());
}
