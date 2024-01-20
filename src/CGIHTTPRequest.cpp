#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "CGIHTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTTPResponse404.hpp"
#include "HTTPResponse500.hpp"

CGIHTTPRequest::CGIHTTPRequest(
	const ServerConfig &serverConfig,
	const LocationConfig &location,
	const HTTPHeader &httpHeader) : HTTPRequest(serverConfig, location, httpHeader)
{
}

CGIHTTPRequest::CGIHTTPRequest(const CGIHTTPRequest &src)
	: HTTPRequest(src.serverConfig, src.location, src.httpHeader)
{
	*this = src;
}

CGIHTTPRequest::~CGIHTTPRequest() {}

CGIHTTPRequest &CGIHTTPRequest::operator=(const CGIHTTPRequest &rhs)
{
	(void)rhs;
	return (*this);
}

// We need a way to clone a derived instance not knowing its derived form
CGIHTTPRequest *CGIHTTPRequest::clone()
{
	return (new CGIHTTPRequest(*this));
}

HTTPResponse CGIHTTPRequest::process()
{
	HTTPResponse response;

	try
	{
		std::string bodyContent = "<!DOCTYPE html><html><body><h1> CGIHTTPRequest </h1><p> CGI request served from location with path = " + this->location.getUploadPath() + " </p></body></html>";
		std::stringstream ss;

		ss << "HTTP/1.1 200 OK\r\n"
		   << "Content-Type: text/html\r\n"
		   << "Content-Length: " << bodyContent.size()
		   << "\r\n\r\n"
		   << bodyContent;

		if (false /* check if file not found. Comment this if to test 404 */)
		{
			return HTTPResponse404();
		}

		response.setResponse(ss.str());
	}
	catch (...)
	{
		return HTTPResponse500();
	}

	return (response);
}
