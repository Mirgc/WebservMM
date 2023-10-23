#include "ConfigFileParser.hpp"

ConfigFileParser::ConfigFileParser(const std::string &configFile): _fileName(configFile){
}

ConfigFileParser::~ConfigFileParser(){
}

void ConfigFileParser::checkFile(){
	struct stat	fileInfo; //Estructura que nos da informacion del fichero

        if (stat(_fileName.c_str(), &fileInfo) != -1) // Si al intentar sacar el estado da error
        {
                if (fileInfo.st_mode & S_IFREG) { // Si es una ruta pero vacia, sin indicarle fichero
			if (access(_fileName.c_str(), R_OK) == 0) { //Tenemos permisos de lectura en el fichero
			        if (isEmptyFile()) // El fichero esta vacio o contiene solo espacios en blanco
					throw ParseException("File is empty.");
				else
					removeCommentsWhiteLines();
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
	std::ifstream	file(_fileName.c_str());
	char		ch;

	while (file.get(ch)){
		if(!isspace(static_cast<unsigned char>(ch))){
			return false;
			file.close();
		}
	}
	file.close();
	return true;
}

void ConfigFileParser::removeCommentsWhiteLines(){
	std::ifstream		input(_fileName);
	std::ostringstream	output;
	std::string		line;
	size_t			pos;

	while (std::getline(input, line)){ // Eliminar espacios en blanco y tabulaciones al principio y al final de la línea
		size_t start = line.find_first_not_of(" \t");
		size_t end = line.find_last_not_of(" \t");

		if (start != std::string::npos && end != std::string::npos)
			line = line.substr(start, end - start + 1);
		else
			line.clear(); // La línea solo tiene espacios y tabulaciones, se convierte en línea vacía

		output << line << std::endl;
	}
	_fileContent = output.str();

	// Eliminamos comentarios
        pos = 0;
        while ((pos = _fileContent.find("#", pos)) != std::string::npos)
                _fileContent.erase(pos, _fileContent.find("\n", pos + 1) - pos);

	// Eliminamos lineas vacias
	pos = 0;
	while ((pos = _fileContent.find("\n\n", pos)) != std::string::npos)
		_fileContent.erase(pos, 1);

    	input.close();
}
