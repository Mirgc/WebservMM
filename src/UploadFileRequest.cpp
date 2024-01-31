#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#include "UploadFileRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTTPResponse404.hpp"
#include "HTTPResponse500.hpp"
#include "MIMETypes.hpp"

UploadFileRequest::UploadFileRequest(
	const ServerConfig &serverConfig,
	const LocationConfig &location,
	const HTTPHeader &httpHeader,
    const HTTPBody & httpBody
) : HTTPRequest(serverConfig, location, httpHeader), httpBody(httpBody)
{
}

UploadFileRequest::UploadFileRequest(const UploadFileRequest &src)
	: HTTPRequest(src.serverConfig, src.location, src.httpHeader), httpBody(src.httpBody)
{
	*this = src;
}

UploadFileRequest::~UploadFileRequest() {}

UploadFileRequest &UploadFileRequest::operator=(const UploadFileRequest &rhs)
{
    (void)rhs;
    return (*this);
}

// We need a way to clone a derived instance not knowing its derived form
UploadFileRequest *UploadFileRequest::clone()
{
    return (new UploadFileRequest(*this));
}

HTTPResponse UploadFileRequest::process()
{
    HTTPResponse response;

    return (response);
}
