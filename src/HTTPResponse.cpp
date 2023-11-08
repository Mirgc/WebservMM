#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse() {}

HTTPResponse::HTTPResponse(const HTTPResponse & src) {
    *this = src;
}

HTTPResponse::~HTTPResponse() {}

HTTPResponse& HTTPResponse::operator=(const HTTPResponse &rhs) {
	if (this != &rhs) {
        this->setResponse(rhs.response);
    }

	return (*this);
}

const std::string HTTPResponse::getResponse() {
	return (this->response);
}

void HTTPResponse::setResponse(const std::string & response) {
	this->response = response;
}
