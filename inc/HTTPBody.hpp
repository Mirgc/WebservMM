#ifndef HTTP_BODY_H
#define HTTP_BODY_H

#include <iostream>
#include <vector>
#include <string>

class HTTPBody
{
	public:
		HTTPBody(void);
		HTTPBody(const HTTPBody & src);
		HTTPBody & operator=(HTTPBody const & src);
		~HTTPBody(void);

		void			addBodyChunk(const std::string & bodyChunk);
		std::string &	getFullBody(void);

	private:
		std::string body;

};

#endif
