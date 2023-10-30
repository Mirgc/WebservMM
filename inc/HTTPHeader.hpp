#ifndef HTTPHEADER_H
#define HTTPHEADER_H
#include <iostream>
#include <vector>
#include <string>

class HTTPHeader
{
	private:
		std::vector<std::pair<std::string, std::string> > header;	
	public:
		HTTPHeader(void);
		HTTPHeader(const HTTPHeader & src);
		HTTPHeader & operator=(HTTPHeader const & src);
		~HTTPHeader(void);

		void		addHeader(const std::string & value1, const std::string & value2);
		void		printHeader(void) const;
		std::string	generateResponse(void) const;
};

#endif
