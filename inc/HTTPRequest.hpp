#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "LocationConfig.hpp"

class HTTPResponse;

class HTTPRequest {
public:
    HTTPRequest(const LocationConfig & location);
    HTTPRequest(const HTTPRequest & src);
    virtual ~HTTPRequest();

    HTTPRequest & operator=(HTTPRequest const & rhs);

    // We need a way to clone a derived instance not knowing its derived form
    virtual HTTPRequest* clone() = 0;
    virtual HTTPResponse process() = 0;

    LocationConfig location;

};

#endif
