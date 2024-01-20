#ifndef CGI_HTTP_REQUEST_HPP
#define CGI_HTTP_REQUEST_HPP

#include "HTTPRequest.hpp"

class HTTPResponse;
class LocationConfig;

class CGIHTTPRequest : public HTTPRequest
{
public:
	CGIHTTPRequest(
		const ServerConfig &serverConfig,
		const LocationConfig &location,
		const HTTPHeader &httpHeader);
	CGIHTTPRequest(const CGIHTTPRequest &src);
	~CGIHTTPRequest();

	CGIHTTPRequest &operator=(CGIHTTPRequest const &rhs);

	// We need a way to clone a derived instance not knowing its derived form
	CGIHTTPRequest *clone();

	HTTPResponse process();
};

#endif
