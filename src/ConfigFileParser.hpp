#ifndef CONFIGFILEPARSER_H
# define CONFIGFILEPARSER_H

# include <string>
# include <iostream>
# include <sys/stat.h>
# include <unistd.h>
# include <fstream>

class ConfigFileParser{
	private:
		std::string _filename;
	public:
		ConfigFileParser(const std::string &configFile);
		~ConfigFileParser();

		void checkFile();
		bool isEmptyFile();

		class ParseException : public std::exception
		{
			private:
				std::string _message;
			public:
				ParseException(std::string message) throw(){
					_message = "ERROR: " + message;
				}
				virtual const char* what() const throw(){
					return (_message.c_str());
				}
				virtual ~ParseException() throw() {}
		};
};

#endif
