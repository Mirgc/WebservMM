#include "ConfigFileParser.hpp"

ConfigFileParser::ConfigFileParser(const std::string &configFile): _filename(configFile){
	std::cout << this->_filename << std::endl;
}

ConfigFileParser::~ConfigFileParser(){
}

void ConfigFileParser::checkFile(){
	struct stat	fileInfo; //Estructura que nos da informacion del fichero

        if (stat(_filename.c_str(), &fileInfo) != -1) // Si al intentar sacar el estado da error
        {
                if (fileInfo.st_mode & S_IFREG) { // Si es una ruta pero vacia, sin indicarle fichero
			if (access(_filename.c_str(), R_OK) == 0) { //Tenemos permisos de lectura en el fichero
			        if (isEmptyFile()) // El fichero esta vacio o contiene solo espacios en blanco
					throw ParseException("File is empty.");
				else
					std::cout << "TODO OK" << std::endl;
		    	}
			else
				throw ParseException("Access denied.");
		}
                else
			throw ParseException("File is invalid");
        }
        else
		throw ParseException("File is invalid");

}

bool ConfigFileParser::isEmptyFile(){
	std::ifstream file(_filename.c_str());
	char ch;
	while (file.get(ch)){
		if(!isspace(static_cast<unsigned char>(ch)))
			return false;
	}
	return true;
}

