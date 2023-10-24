#ifndef CONFIGFILEPARSER_H
# define CONFIGFILEPARSER_H

# include <string>
# include <iostream>
# include <sys/stat.h>
# include <unistd.h>
# include <fstream>
# include <sstream>

class ConfigFileParser{
	private:
		std::string _fileContent;

	public:
		ConfigFileParser();
		~ConfigFileParser();

		void checkFile(const std::string &fileName);
		bool isEmptyFile(const std::string &fileName);
		std::string removeCommentsWhiteLines(const std::string &fileName);

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
