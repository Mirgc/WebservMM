#include "HTTPHeader.hpp"
#include <sstream>

HTTPHeader::HTTPHeader(void) {
}

HTTPHeader::~HTTPHeader(void) {
}

HTTPHeader::HTTPHeader(const HTTPHeader & src) {
	*this = src;
}

HTTPHeader & HTTPHeader::operator = (HTTPHeader const & src) {
	if (this != &src) {
		this->header = src.header;
	}
	return (*this);
}

void	HTTPHeader::addMethod(std::string line) {
	std::istringstream lineStream(line);
	lineStream >> this->method >> this->url;
}
//Could be null values?? if so, we can check value1 & value2  anf throw exception
void	HTTPHeader::addHeader(const std::string & value1, const std::string & value2) {
	this->header.push_back(std::make_pair(value1, value2));
}

void	HTTPHeader::printHeader(void) const {
	std::cout << "Method: " << this->method << std::endl;
    std::cout << "URL: " << this->url << std::endl;
	for (size_t i = 0; i< this->header.size(); ++i) {
		std::cout << this->header[i].first << ": " << this->header[i].second;
		std::cout << std::endl;
	}
}

std::string	HTTPHeader::generateResponse(void) const {
	std::string	response;

	response += this->method + " " + this->url + "\n"; //HTTP status
	for (size_t i = 0; i< this->header.size(); ++i) {
		response += this->header[i].first + ": " + this->header[i].second + "\n";
	}
	return response;
}

void HTTPHeader::parseHTTPHeader (const std::string& request) {
    std::istringstream iss(request);
    std::string line;
	while (std::getline(iss, line)) {
        // Analizar la primera línea para obtener el método y la URL
        if (this->method.empty()) {
            this->addMethod(line);
        }
        // Analizar las líneas siguientes para obtener los encabezados
        else {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 2); // +2 para omitir el ': ' después del encabezado
                this->addHeader(key, value);
            }
        }
    }
}
