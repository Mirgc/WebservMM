#include "HTTPRequestFactory.hpp"
#include "StaticFileHTTPRequest.hpp"

HTTPRequestFactory::HTTPRequestFactory() {}

HTTPRequestFactory::HTTPRequestFactory(const HTTPRequestFactory & src) {
    *this = src;
}

HTTPRequestFactory::~HTTPRequestFactory() {}

HTTPRequestFactory& HTTPRequestFactory::operator=(const HTTPRequestFactory &rhs) {
	if (this != &rhs) {

    }

	return (*this);
}

HTTPRequest * HTTPRequestFactory::createHTTPRequest(/* parameters needed */) {
    // TODO: add parameters so that the factory knows what concrete instance should create
    return (new StaticFileHTTPRequest());
}
