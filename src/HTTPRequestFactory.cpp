#include "HTTPRequestFactory.hpp"
#include "StaticFileHTTPRequest.hpp"

HTTPRequestFactory::HTTPRequestFactory() {}

HTTPRequestFactory::HTTPRequestFactory(const HTTPRequestFactory & src) {
    *this = src;
}

HTTPRequestFactory::~HTTPRequestFactory() {}

HTTPRequestFactory& HTTPRequestFactory::operator=(const HTTPRequestFactory &rhs) {
    (void) rhs;
	return (*this);
}

HTTPRequest * HTTPRequestFactory::createHTTPRequest(/* parameters needed */) {
    // TODO: add parameters so that the factory knows what concrete instance should create

    // Types could be:
    // Static file request; find a file, read it, send content to client
    // Redirection request; Just send HTTP 3xx redirection headers to the client
    // CGI request; call the CGI and return its response to the client
    // Directory listing request; If directory, read all files and directories in it, produce an HTML page with it
    // Upload request
    // Any other?

    return (new StaticFileHTTPRequest());
}
