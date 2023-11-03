#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

class HTTPResponse;

class HTTPRequest {
public:
    HTTPRequest();
    HTTPRequest(const HTTPRequest & src);
    virtual ~HTTPRequest();

    HTTPRequest & operator=(HTTPRequest const & rhs);

    virtual HTTPResponse process() = 0;
};

#endif
