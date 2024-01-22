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

bool esArchivo(const std::string &ruta)
{
    struct stat info;
    return stat(ruta.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}

bool esDirectorio(const std::string &ruta)
{
    DIR *directorio = opendir(ruta.c_str());
    if (directorio != NULL)
    {
        closedir(directorio);
        return true;
    }
    return false;
}

std::string getContentType(std::string str)
{
    std::map<std::string, std::string> extensionToContentType;
    extensionToContentType[".aac"] = "audio/aac";
    extensionToContentType[".abw"] = "application/x-abiword";
    extensionToContentType[".arc"] = "application/octet-stream";
    extensionToContentType[".avi"] = "video/x-msvideo";
    extensionToContentType[".azw"] = "application/vnd.amazon.ebook";
    extensionToContentType[".bin"] = "application/octet-stream";
    extensionToContentType[".bz"] = "application/x-bzip";
    extensionToContentType[".bz2"] = "application/x-bzip2";
    extensionToContentType[".csh"] = "application/x-csh";
    extensionToContentType[".css"] = "text/css";
    extensionToContentType[".csv"] = "text/csv";
    extensionToContentType[".doc"] = "application/msword";
    extensionToContentType[".eot"] = "application/vnd.ms-fontobject";
    extensionToContentType[".epub"] = "application/epub+zip";
    extensionToContentType[".gif"] = "image/gif";
    extensionToContentType[".htm"] = "text/html";
    extensionToContentType[".html"] = "text/html";
    extensionToContentType[".ico"] = "image/x-icon";
    extensionToContentType[".ics"] = "text/calendar";
    extensionToContentType[".jar"] = "application/java-archive";
    extensionToContentType[".jpeg"] = "image/jpeg";
    extensionToContentType[".jpg"] = "image/jpeg";
    extensionToContentType[".js"] = "application/javascript";
    extensionToContentType[".json"] = "application/json";
    extensionToContentType[".mid"] = "audio/midi";
    extensionToContentType[".midi"] = "audio/midi";
    extensionToContentType[".mpeg"] = "video/mpeg";
    extensionToContentType[".mpkg"] = "application/vnd.apple.installer+xml";
    extensionToContentType[".odp"] = "application/vnd.oasis.opendocument.presentation";
    extensionToContentType[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    extensionToContentType[".odt"] = "application/vnd.oasis.opendocument.text";
    extensionToContentType[".oga"] = "audio/ogg";
    extensionToContentType[".ogv"] = "video/ogg";
    extensionToContentType[".ogx"] = "application/ogg";
    extensionToContentType[".otf"] = "font/otf";
    extensionToContentType[".png"] = "image/png";
    extensionToContentType[".pdf"] = "application/pdf";
    extensionToContentType[".ppt"] = "application/vnd.ms-powerpoint";
    extensionToContentType[".rar"] = "application/x-rar-compressed";
    extensionToContentType[".rtf"] = "application/rtf";
    extensionToContentType[".sh"] = "application/x-sh";
    extensionToContentType[".svg"] = "image/svg+xml";
    extensionToContentType[".swf"] = "application/x-shockwave-flash";
    extensionToContentType[".tar"] = "application/x-tar";
    extensionToContentType[".tif"] = "image/tiff";
    extensionToContentType[".tiff"] = "image/tiff";
    extensionToContentType[".ts"] = "video/vnd.dlna.mpeg-tts";
    extensionToContentType[".ttf"] = "font/ttf";
    extensionToContentType[".vsd"] = "application/vnd.visio";
    extensionToContentType[".wav"] = "audio/x-wav";
    extensionToContentType[".weba"] = "audio/webm";
    extensionToContentType[".webm"] = "video/webm";
    extensionToContentType[".webp"] = "image/webp";
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

std::string getReponse(const std::string& path)
{
    std::string contenido;
    int descriptorArchivo = open(path.c_str(), O_RDONLY);
    if (descriptorArchivo == -1)
    {
        throw std::runtime_error("Failed to create a listening socket");
    }
    char buffer[4096]; // Puedes ajustar el tamaño del buffer según tus necesidades
    ssize_t tmp;
    ssize_t bytesLeidos = 0;
    while ((tmp = read(descriptorArchivo, buffer, sizeof(buffer))) > 0)
    {
        bytesLeidos += tmp;
        contenido.append(buffer, bytesLeidos);
    }
    if (bytesLeidos == -1)
    {
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

std::string intToString(int value) {
    char buffer[20];  // Suficientemente grande para almacenar números enteros comunes
    std::sprintf(buffer, "%d", value);
    return std::string(buffer);
}

std::string generateAutoindexPage(const std::string& directoryPath) {
    
    std::string htmlPage = "<html><head></head><body>\n";

    // hay que revisar cuando nos movemos por los archivos lo que pasa.
    // Lee el contenido del directorio
    DIR* dir = opendir(directoryPath.c_str());
    if (dir != NULL) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            htmlPage += "<li><a href=\"" + std::string(entry->d_name) + "\">" + std::string(entry->d_name) + "</a></li>";
            std::cout << directoryPath << std::string(entry->d_name) << std::endl;
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

std::string obtenerParteDespuesDeTerceraBarra(const std::string& url) {
    size_t posPrimeraBarra = url.find('/');
    size_t posSegundaBarra = url.find('/', posPrimeraBarra + 1);
    size_t posTerceraBarra = url.find('/', posSegundaBarra + 1);

    if (posTerceraBarra != std::string::npos) {
        return url.substr(posTerceraBarra + 1);
    } else {
        // No se encontró la tercera barra, puedes manejar este caso según tus necesidades.
        return "";
    }
}

void removeSubstring(std::string& mainString, const std::string& substringToRemove) {
    size_t pos = mainString.find(substringToRemove);
    if (pos != std::string::npos) {
        mainString.erase(pos, substringToRemove.length());
    }
}

std::string StaticFileHTTPRequest::funciontemporal()
{
    std::string rutaCompleta;
            rutaCompleta = this->httpHeader.getHeader("Referer");
            rutaCompleta = "/" + obtenerParteDespuesDeTerceraBarra(rutaCompleta);
            std::string tmp = this->httpHeader.getUrl();
            removeSubstring(tmp, rutaCompleta);
            rutaCompleta = "./" + tmp;
            return rutaCompleta;
}

HTTPResponse StaticFileHTTPRequest::process()
{
    HTTPResponse response;

    // Tengo que asegurarme que al concatenar los paths, tnegan / metida entre medias.
    try
    {
        std::string rutaCompleta;
        if (this->httpHeader.isMethod("Referer") && false)
        {
            rutaCompleta = this->funciontemporal();
        }
        else
            rutaCompleta = this->location.getCfgValueFrom("docroot") + this->httpHeader.getUrl();
        //std::cout << "\n\n\n\n\n\n\n\n\n\n " << rutaCompleta << "\n\n\n\n\n\n\n";
        if (esArchivo(rutaCompleta))
        {
            response.setResponse(getReponse(rutaCompleta));
        }
        else if (esDirectorio(rutaCompleta))
        {
            size_t ultimaPosicion = rutaCompleta.find_last_of('/');
            if (ultimaPosicion != std::string::npos && ultimaPosicion != rutaCompleta.length() - 1) {
                rutaCompleta += "/";
            }
            if (true)
            { // De momento entra siempre aqui, pero aqui ira la logica de autoindex
                rutaCompleta += this->location.getCfgValueFrom("index");
                if (esArchivo(rutaCompleta))
                {
                    response.setResponse(getReponse(rutaCompleta));
                }
                else
                    return HTTPResponse404();
            }
            else 
            {
                response.setResponse(generateAutoindexPage(rutaCompleta));
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
