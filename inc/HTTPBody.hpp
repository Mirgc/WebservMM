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

		void                    addBodyChunk(const std::string & bodyChunk);
		const std::string &     getFullBody(void) const;
        std::size_t             getBodySize(void) const;
        std::string             getBodySizeString(void) const;

	private:
		std::string body;

};

#endif
