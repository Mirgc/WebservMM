#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "StaticFileHTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTTPResponse404.hpp"
#include "HTTPResponse500.hpp"

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

bool isFile(const std::string& path) {
    FILE* file = fopen(path.c_str(), "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

bool isValidDirectory(const std::string& path) {
    // Puedes agregar lógica adicional aquí para verificar si es un directorio válido.
    // En este ejemplo, simplemente se comprueba si es un directorio no vacío.
    return isFile(path);
}

bool isValidExtension(const std::string& fileName, const std::string& validExtension) {
    size_t pos = fileName.rfind('.');
    if (pos != std::string::npos) {
        std::string extension = fileName.substr(pos + 1);
        return (extension == validExtension);
    }
    return false;
}

HTTPResponse StaticFileHTTPRequest::process()
{
    HTTPResponse response;

    try
    {
        std::string archivo2 = this->httpHeader.getUrl();
        std::ifstream archivo(this->httpHeader.getUrl());
          
        if (!isFile(archivo2)) {
            throw std::runtime_error("No se pudo abrir el archivo: " + archivo2);
        }
        std::string bodyContent = "<!DOCTYPE html><html><body><h1> StaticFileHTTPRequest </h1><p> Static file served from location with path = " + this->location.getUploadPath() + " </p></body></html>";
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
    catch (const std::runtime_error& e) 
    {
        std::cerr << "Error de tiempo de ejecución: " << e.what() << std::endl;
        return HTTPResponse500();

    }
    catch (...)
    {
        return (HTTPResponse404());
    }
    return (response);
}
