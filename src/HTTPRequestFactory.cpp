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

    (void)httpBody;

    const LocationConfig &location = this->getLocationWithRequest(serverConfig, httpHeader);
    // TODO: Check it is a valid verb and if not, return a 405 Method Not Allowed

    return (new StaticFileHTTPRequest(serverConfig, location, httpHeader));
}

const LocationConfig HTTPRequestFactory::getLocationWithRequest(
    const ServerConfig &serverConfig,
    const HTTPHeader &httpHeader
)
{
    std::vector<LocationConfig> locations = serverConfig.getLocations();
    for (size_t i = 0; i < locations.size(); ++i) {
        // TODO: We need to identify CGI locations
        // Checks for cgi extension match
        // if (httpHeader.getExtension() == locations[i].getPath()) {
        //     return locations[i];
        // }

        // Checks for part starting with
        if (httpHeader.getUrl().find(locations[i].getUploadPath()) == 0) {
            return locations[i];
        }
    }

    throw std::runtime_error("Location not found for path: " + httpHeader.getUrl());
}

