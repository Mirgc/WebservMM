#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#include "StaticFileHTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTTPResponse404.hpp"
#include "HTTPResponse500.hpp"
#include "MIMETypes.hpp"

StaticFileHTTPRequest::StaticFileHTTPRequest(
    const ServerConfig &serverConfig,
    const LocationConfig &location,
    const HTTPHeader &httpHeader) : HTTPRequest(serverConfig, location, httpHeader)
{
}

StaticFileHTTPRequest::StaticFileHTTPRequest(const StaticFileHTTPRequest &src)
    : HTTPRequest(src.serverConfig, src.location, src.httpHeader)
{
    *this = src;
}

StaticFileHTTPRequest::~StaticFileHTTPRequest() {}

StaticFileHTTPRequest &StaticFileHTTPRequest::operator=(const StaticFileHTTPRequest &rhs)
{
    (void)rhs;
    return (*this);
}

// We need a way to clone a derived instance not knowing its derived form
StaticFileHTTPRequest *StaticFileHTTPRequest::clone()
{
    return (new StaticFileHTTPRequest(*this));
}

bool isFile(const std::string &path)
{
    struct stat info;
    return stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}

bool isDirectory(const std::string &path)
{
    DIR *directory = opendir(path.c_str());
    if (directory != NULL)
    {
        closedir(directory);
        return true;
    }
    return false;
}

std::string getResponse(const std::string& path)
{
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        throw std::runtime_error("Failed to open the file");
    }

    std::stringstream ss;
    const int bufferSize = 4096;
    char buffer[bufferSize];
    ssize_t bytesReaded;

    while ((bytesReaded = read(fd, buffer, sizeof(buffer))) > 0) {
        ss.write(buffer, bytesReaded);
    }

    if (bytesReaded == -1) {
        throw std::runtime_error("Failed to create a listening socket");
    }

    close(fd);

    std::string content = ss.str();
    
    ss.str("");  // Clean up the stringstream to reuse it
    ss << "HTTP/1.1 200 OK\r\n"
       << "Content-Type: "
       << MIMETypes::getContentType(path)
       << "\r\n"
       << "Content-Length: " << content.size()
       << "\r\n\r\n"
       << content;

    return ss.str();

}

std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void removeSubstring(std::string& mainString, const std::string& substringToRemove) {
    size_t pos = mainString.find(substringToRemove);
    if (pos != std::string::npos) {
        mainString.erase(pos, substringToRemove.length());
    }
}

std::string StaticFileHTTPRequest::generateAutoindexPage(const std::string& directoryPath) {
    
    std::string htmlPage = "<html><head></head><body>\n";

    // we have to check what happens when we move through the files.
    // Read the contents of the directory
    DIR* dir = opendir(directoryPath.c_str());
    std::string dc = directoryPath.c_str();
    removeSubstring(dc, this->location.getCfgValueFrom("docroot"));
    size_t lastPosition = dc.find_last_of('/');
    if (lastPosition != std::string::npos && lastPosition != dc.length() - 1) {
        dc += "/";
    }
    if (dir != NULL) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            htmlPage += "<li><a href=\"" + dc + std::string(entry->d_name) + "\">" + std::string(entry->d_name) + "</a></li>";
        }

        closedir(dir);
    } else {
        throw std::runtime_error("Failed to create a listening socket");
    }

    htmlPage += "</body></html>";
    int length = static_cast<int>(htmlPage.length());
    std::string htmlPageF = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + intToString(length) + "\r\n\r\n" + htmlPage;
    return htmlPageF;
}

HTTPResponse StaticFileHTTPRequest::process()
{
    HTTPResponse response;
    std::string pathComplete;
    // I have to make sure that when concatenating the paths, there is no / tucked in between.
    try
    {
        pathComplete = this->location.getCfgValueFrom("docroot") + this->httpHeader.getUrl();
        if (isFile(pathComplete))
        {
            response.setResponse(getResponse(pathComplete));
        }
        else if (isDirectory(pathComplete))
        {
            size_t lastPosition = pathComplete.find_last_of('/');
            if (lastPosition != std::string::npos && lastPosition != pathComplete.length() - 1) {
                pathComplete += "/";
            }
            if (!this->location.getAutoIndexBool())
            {
                pathComplete += this->location.getCfgValueFrom("index");
                if (isFile(pathComplete))
                {
                    response.setResponse(getResponse(pathComplete));
                }
                else
                    return HTTPResponse404(this->serverConfig.get404Content());
            }
            else 
            {
                response.setResponse(this->generateAutoindexPage(pathComplete));
            }
        }
        else
            return HTTPResponse404(this->serverConfig.get404Content());
        return response;
    }
    catch (...)
    {
        return (HTTPResponse500(this->serverConfig.get500Content()));
    }
    return (response);
}
