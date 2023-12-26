#ifndef STATIC_FILE_HTTP_REQUEST_HPP
#define STATIC_FILE_HTTP_REQUEST_HPP

#include "HTTPRequest.hpp"

class HTTPResponse;
class LocationConfig;

class StaticFileHTTPRequest: public HTTPRequest {
public:
    StaticFileHTTPRequest(const LocationConfig & location);
    StaticFileHTTPRequest(const StaticFileHTTPRequest & src);
    ~StaticFileHTTPRequest();

    StaticFileHTTPRequest & operator=(StaticFileHTTPRequest const & rhs);

    // We need a way to clone a derived instance not knowing its derived form
    StaticFileHTTPRequest * clone();

    HTTPResponse process();

};

#endif
