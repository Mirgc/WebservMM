#include "HTTPHeader.hpp"
#include <sstream>

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

std::string ltrim(std::string &s)
{
	const std::string WHITESPACE = " \n\r\t";
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
	const std::string WHITESPACE = " \n\r\t";
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

void HTTPHeader::addMethod(std::string line)
{
	std::istringstream lineStream(line);
	lineStream >> this->method >> this->url >> this->ver;
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
	// Aqui vamos a revisar que la version es la correcta. Se podria revisar que el metodo fuera valido.
	if (!this->ver.compare("HTTP/1.1"))
		return true;
	return false;
}

void	HTTPHeader::insertBody(std::isstringstream)

void HTTPHeader::parseHTTPHeader(const std::string &request)
{
	std::istringstream iss(request);
	std::string line;
	std::string tmp;

	std::getline(iss, line);
	this->addMethod(line);
	if (!this->checkMethod())
	{
		std::cout << "Not HTTP correct version" << std::endl;
		return; // Aqui habria que lanzar una excepcion
	}

	while (std::getline(iss, line))
	{
		// Analizar la primera línea para obtener el método y la URL
		line = rtrim(ltrim(line));
		size_t pos = line.find(':');
		if (pos != std::string::npos && pos > 0 && pos < line.length() - 1)
		{
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 2); // +2 para omitir el ': ' después del encabezado
			this->addHeader(rtrim(key), ltrim(value)); // el rtrim(key), en la realidad, no se hace.
		}
		else
		{
			if (line.compare(""))
				std::cout << "Mala descripcion" << std::endl;
			else
				
				std::cout << "checkbody" << std::endl;
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
