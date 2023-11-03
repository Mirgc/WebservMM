#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>

class HTTPResponse {
public:
    HTTPResponse();
    HTTPResponse(const HTTPResponse & src);
    virtual ~HTTPResponse();

    HTTPResponse & operator=(HTTPResponse const & rhs);

    void setResponse(const std::string & response);
    const std::string getResponse();

private:

    std::string response;
};

#endif
