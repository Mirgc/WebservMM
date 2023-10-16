#include "ConfigFileParser.hpp"

int main(int argc, char **argv){
	if (argc <= 2){
		try{
			std::string configFile;
			if (argc == 2){
				configFile=argv[1];
			}
			else{
				configFile=("./httpd.conf");
			}
			ConfigFileParser parse(configFile);
			parse.checkFile();
		}
		catch (std::exception &e){
			std::cerr << e.what() << std::endl;
			return (1);
		}	
	}
	else {
		std::cout << "ERROR: Wrong parameters." << std::endl;
		return (1);
	}
	return (0);
}
