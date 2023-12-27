#include "ConfigFileParser.hpp"

ConfigFileParser::ConfigFileParser(){
}

ConfigFileParser::~ConfigFileParser(){
}

// TODO: Change checkfile method this fails too many times
void ConfigFileParser::checkFile(const std::string &fileName){
	struct stat	fileInfo; //Estructura que nos da informacion del fichero

        if (stat(fileName.c_str(), &fileInfo) != -1){ // Si al intentar sacar el estado da error
			if (fileInfo.st_mode & S_IFREG) { // Si es una ruta pero vacia, sin indicarle fichero
				if (access(fileName.c_str(), R_OK) == 0) { //Tenemos permisos de lectura en el fichero
			        if (isEmptyFile(fileName)) // El fichero esta vacio o contiene solo espacios en blanco
						throw ParseException("File is empty.");
					else{
						this->_fileContent = removeCommentsWhiteLines(fileName);
						splitServers();
						eachServerCfg();
					}
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

bool ConfigFileParser::isEmptyFile(const std::string &fileName){
	std::ifstream	file(fileName.c_str());
	char			ch;

	while (file.get(ch)){
		if(!isspace(static_cast<unsigned char>(ch))){
			file.close();
			return false;
		}
	}
	file.close();
	return true;
}

std::string ConfigFileParser::removeCommentsWhiteLines(const std::string &fileName){
	std::ifstream		input(fileName.c_str());
	std::ostringstream	output;
	std::string			line;
	std::string			content;
	size_t				pos;

	while (std::getline(input, line)){ // Eliminar espacios en blanco y tabulaciones al principio y al final de la línea
		size_t start = line.find_first_not_of(" \t");
		size_t end = line.find_last_not_of(" \t");

		if (start != std::string::npos && end != std::string::npos)
			line = line.substr(start, end - start + 1);
		else
			line.clear(); // La línea solo tiene espacios y tabulaciones, se convierte en línea vacía

		output << line << std::endl;
	}
	content = output.str();

	// Eliminamos comentarios
        pos = 0;
        while ((pos = content.find("#", pos)) != std::string::npos)
			content.erase(pos, content.find("\n", pos + 1) - pos);

	// Eliminamos lineas vacias
	pos = 0;
	while ((pos = content.find("\n\n", pos)) != std::string::npos)
		content.erase(pos, 1);

    	input.close();
	return content;
}

void ConfigFileParser::splitServers(){
	size_t start = this->_fileContent.find("server");;
	size_t end = 1;	
	while (start != end && start != this->_fileContent.length()){
		if (this->_fileContent.compare(start, 6, "server") != 0)
			throw ParseException("File is invalid");
		start += 6;
		while (this->_fileContent[start] && isspace(this->_fileContent[start]))
			start++;
		if (this->_fileContent[start] != '{')
			throw ParseException("File is invalid");

		size_t j = 1;
		for (size_t i = start + 1; this->_fileContent[i]; i++){
			if (this->_fileContent[i] == '{')
				j++;
			if (this->_fileContent[i] == '}'){
				j--;
				if (!j){
					end = i + 1;
					break;
				}
			}
		}
		if (j != 0)
        		throw ParseException("No valid server blocks found in the file");
		this->_serverConfig.push_back(this->_fileContent.substr(start, end - start + 1));
		this->_numServ++;
		start = end + 1;	
	}
}

const std::vector<std::string> & ConfigFileParser::getServerCfg(void) const{
	return(this->_serverConfig);
}

std::ostream & operator<<(std::ostream & o, std::vector<std::string> const & rhs){
	for(std::vector<std::string>::const_iterator it = rhs.begin();
	it != rhs.end(); ++it)
		std::cout << (*it) << std::endl;
	return (o);
}

void ConfigFileParser::eachServerCfg(){

	std::vector<std::string>::iterator start;
	std::vector<std::string>::iterator end;
	int index = 0;
	
	end = this->_serverConfig.end();

	for(start = this->_serverConfig.begin(); start != end; start++){
			std::istringstream split(*start);
			std::vector<std::string> tokens;
			for (std::string each; std::getline(split, each, '\n'); tokens.push_back(each));
			this->_eachServer[index] = tokens;
			index++;
	}
}
