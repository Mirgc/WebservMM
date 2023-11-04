#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest() {}

HTTPRequest::HTTPRequest(const HTTPRequest & src) {
    *this = src;
}

HTTPRequest::~HTTPRequest() {}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest &rhs) {
    (void) rhs;
	return (*this);
}
