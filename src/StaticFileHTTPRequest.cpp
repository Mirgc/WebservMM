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

bool esArchivo(const std::string& ruta) {
    struct stat info;
    return stat(ruta.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}

bool esDirectorio(const std::string& ruta) {
    DIR* directorio = opendir(ruta.c_str());
    if (directorio != NULL) {
        closedir(directorio);
        return true;
    }
    return false;
}

std::string getContentType(std::string str)
{
    std::map<std::string, std::string> extensionToContentType;
    extensionToContentType[".html"] = "text/html";
    extensionToContentType[".css"] = "text/css";
    extensionToContentType[".js"] = "application/javascript";
    extensionToContentType[".jpg"] = "image/jpeg";
    extensionToContentType[".png"] = "image/png";
    // Extract the file extension from the filename
    size_t dotPosition = str.find_last_of(".");
    if (dotPosition != std::string::npos)
    {
        std::string ctExtension = str.substr(dotPosition);
        // Look up the extension in the map
        std::map<std::string, std::string>::iterator it = extensionToContentType.find(ctExtension);
        if (it != extensionToContentType.end())
        {
            return it->second;
        }
    }
    return "application/octet-stream";
}

std::string getReponse(std::string path)
{
    std::string contenido;
    int descriptorArchivo = open(path.c_str(), O_RDONLY);
    if (descriptorArchivo == -1) {
        throw std::runtime_error("Failed to create a listening socket");
    }
    char buffer[4096]; // Puedes ajustar el tamaño del buffer según tus necesidades
    ssize_t tmp;
    ssize_t bytesLeidos = 0;
    while ((tmp = read(descriptorArchivo, buffer, sizeof(buffer))) > 0) {
        bytesLeidos += tmp;
        contenido.append(buffer, bytesLeidos);
    }
    if (bytesLeidos == -1) {
        throw std::runtime_error("Failed to create a listening socket");
    }
    close(descriptorArchivo);
    std::stringstream ss;
    ss << "HTTP/1.1 200 OK\r\n"
    << "Content-Type: "
    << getContentType(path)
    << "\r\n"
    << "Content-Length: " << bytesLeidos
    << "\r\n\r\n"
    << contenido;
    return ss.str();
}

HTTPResponse StaticFileHTTPRequest::process()
{
    HTTPResponse response;

// Tengo que asegurarme que al concatenar los paths, tnegan / metida entre medias.
    try
    {
        std::string rutaCompleta = this->location.getCfgValueFrom("docroot") + this->httpHeader.getUrl();
        if (esArchivo(rutaCompleta)) {
            response.setResponse(getReponse(rutaCompleta));
        }
        else if (esDirectorio(rutaCompleta))
        {
            if (true) {//De momento entra siempre aqui, pero aqui ira la logica de autoindex
                std::string rutaConIndex = rutaCompleta + this->location.getCfgValueFrom("index");
                if (esArchivo(rutaConIndex))
                {
                    response.setResponse(getReponse(rutaConIndex));
                }
                else
                    return HTTPResponse404();
            }
        }
        else
            return HTTPResponse404();
        return response;
    }
    catch (...)
    {
        return (HTTPResponse500());
    }
    return (response);
}
