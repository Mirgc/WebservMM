#include "HTTPRequestFactory.hpp"
#include "StaticFileHTTPRequest.hpp"
#include "LocationConfig.hpp"
#include "ServerConfig.hpp"
#include "HTTPHeader.hpp"
#include "HTTPBody.hpp"

HTTPRequestFactory::HTTPRequestFactory()
{
}

HTTPRequestFactory::HTTPRequestFactory(const HTTPRequestFactory &src)
{
    *this = src;
}

HTTPRequestFactory::~HTTPRequestFactory() {}

HTTPRequestFactory &HTTPRequestFactory::operator=(const HTTPRequestFactory &rhs)
{
    if (this != &rhs)
    {
    }

    return (*this);
}

HTTPRequest *HTTPRequestFactory::createHTTPRequest(const ServerConfig &serverConfig, const HTTPHeader &httpHeader, const HTTPBody &httpBody)
{

    // Types could be:
    // Static file request; find a file, read it, send content to client
    // Redirection request; Just send HTTP 3xx redirection headers to the client
    // CGI request; call the CGI and return its response to the client
    // Directory listing request; If directory, read all files and directories in it, produce an HTML page with it
    // Upload request
    // Any other?

    (void)serverConfig;
    (void)httpHeader;
    (void)httpBody;

    // TODO: We need logic here to identify the right LocationConfig we have to serve from
    LocationConfig location;
    location.setUploadPath("/uploads");

    return (new StaticFileHTTPRequest(location));
}
