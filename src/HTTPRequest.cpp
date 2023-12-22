#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(const LocationConfig & location): location(location) {}

HTTPRequest::HTTPRequest(const HTTPRequest & src) {
    *this = src;
}

HTTPRequest::~HTTPRequest() {}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest &rhs) {
    if (this != &rhs) {
        this->location = rhs.location;
    }

	return (*this);
}
