#include "HTTPHeader.hpp"
#include <sstream>
#include <algorithm> 

HTTPHeader::HTTPHeader(void)
{
}

HTTPHeader::~HTTPHeader(void)
{
}

HTTPHeader::HTTPHeader(const HTTPHeader &src)
{
	*this = src;
}

HTTPHeader &HTTPHeader::operator=(HTTPHeader const &src)
{
	if (this != &src)
	{
		this->header = src.header;
	}
	return (*this);
}

std::string letrim(std::string &s)
{
	const std::string WHITESPACE = " \n\r\t";
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string ritrim(const std::string &s)
{
	const std::string WHITESPACE = " \n\r\t";
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

bool HTTPHeader::addMethod(std::string line)
{
	std::istringstream lineStream(line);
	lineStream >> this->method >> this->url >> this->ver;
	std::string restoDelContenido;
    std::getline(lineStream, restoDelContenido);
	if (restoDelContenido[0] == '\0') // Si hay un espacio ni si quiera despues del 1.1, falla
		return false;
	return true;
}
// Could be null values?? if so, we can check value1 & value2  anf throw exception
void HTTPHeader::addHeader(const std::string &value1, const std::string &value2)
{
	this->header.push_back(std::make_pair(value1, value2));
}

std::string HTTPHeader::generateResponse(void) const
{
	std::string response;

	response += this->method + " " + this->url + "\n"; // HTTP status
	for (size_t i = 0; i < this->header.size(); ++i)
	{
		response += this->header[i].first + ": " + this->header[i].second + "\n";
	}
	return response;
}

bool HTTPHeader::checkMethod(void) const
{
	//t_http_method HttpMethod::getMethod(std::string method)
	// Aqui vamos a revisar que la version es la correcta. Se podria revisar que el metodo fuera valido.
	if (this->method.empty() || this->url.empty() || this->ver.empty() || this->ver.compare("HTTP/1.1") != 0)
		return false;
	const std::string allowedMethods[] = {"GET", "DELETE", "POST"};
    const size_t numMethods = sizeof(allowedMethods) / sizeof(allowedMethods[0]);
    bool ver = false;
    // Verificar si this->method está en el array
    for (size_t i = 0; i < numMethods; ++i) {
        if (allowedMethods[i] == this->method) {
            ver = true;
            break; // Se encontró el método, salir del bucle
        }
    }
	return ver;
}

void HTTPHeader::parseHTTPHeader(const std::string &request)
{
	std::istringstream iss(request);
	std::string line;
	std::string tmp;

	std::getline(iss, line);
	if (this->addMethod(line) || !this->checkMethod())
	{
		std::cout << "Invalid HTTPRequest" << std::endl;
		return; // Aqui habria que lanzar una excepcion
	}
	while (std::getline(iss, line))
	{
		line = ritrim(letrim(line));
		size_t pos = line.find(':');
		if (pos != std::string::npos && pos > 0 && pos < line.length() - 1) //ahora hay que ver si podemos meter vacio algo.
		//que pasa si metemos dos iguales?
		{
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1); // +1 para omitir el ':' después del encabezado
			this->addHeader(ritrim(key), letrim(value)); // el rtrim(key), en la realidad, no se hace.
		}
		else
		{
			if (line.compare(""))
				std::cout << "Mala descripcion" << std::endl;
			else
				//Aqui hemos encontrado una linea vacia, por lo que asumimos que empieza el boy;
				//std::cout << "Checkbody" << std::endl;
				return;
		}
	}
}

void HTTPHeader::printHeader(void) const
{
	std::cout << "Method: " << this->method << std::endl;
	std::cout << "URL: " << this->url << std::endl;
	std::cout << "Version: " << this->ver << std::endl;
	for (size_t i = 0; i < this->header.size(); ++i)
	{
		std::cout << this->header[i].first << ": " << this->header[i].second;
		std::cout << std::endl;
	}
}