#ifndef HTTPHEADER_H
#define HTTPHEADER_H
#include <iostream>
#include <vector>
#include <string>

class HTTPHeader
{
	private:

	public:
		HTTPHeader(void);
		HTTPHeader(const HTTPHeader & src);
		HTTPHeader & operator=(HTTPHeader const & src);
		~HTTPHeader(void);

		std::vector<std::pair<std::string, std::string> > header;
		std::string method;
		std::string url;
		std::string ver;

		void		addHeader(const std::string & value1, const std::string & value2);
		void		addMethod(std::string line);
		void		printHeader(void) const;
		bool		checkMethod(void) const;

		std::string	generateResponse(void) const;
		void		parseHTTPHeader (const std::string& request);

};

#endif
