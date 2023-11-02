#ifndef HTTP_REQUEST_CGI_HPP
#define HTTP_REQUEST_CGI_HPP

#include <string>

class HTTPRequestCGI
{
	public:
		HTTPRequestCGI();
		~HTTPRequestCGI();

		std::string execCGI(std::string cgiScriptRelativePath, std::string queryString);

	private:

		std::string getAbsolutePath(const std::string& relativePath);
		char** createEnvironment(const std::string& cgiScriptAbsolutePath, const std::string& queryString);
		char** createArgs(const std::string& cgiInterpreter, const std::string& cgiScriptAbsolutePath, const std::string& queryString);
		char** createCharPtrArray(const std::vector<std::string>& strings);
		std::string readFromPipe(int pipefd);
};

#endif