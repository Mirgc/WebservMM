#ifndef SERVERCONFIG
#define SERVERCONFIG

# include <map>
# include <string>
# include <vector>
# include <arpa/inet.h>
# include "LocationConfig.hpp"

class ServerConfig{
	private:
		unsigned int			_port;
		in_addr_t			_host;
		std::string			_serverName;
		std::string			_docRoot;
		unsigned int			_clientMaxBodySize;
		std::string			_index;
		std::map<int, std::string>	_errorPageMap;
		std::vector<LocationConfig>	_locations;

        public:
                ServerConfig();
                ~ServerConfig();
                ServerConfig(const ServerConfig &copy);
                ServerConfig &operator=(const ServerConfig &obj);

		void setPort(unsigned int port);
		void setHost(in_addr_t host);
		void setServerName(std::string _serverName);
		void setDocRoot(std::string root);
		void setClientMaxBodySize(unsigned int clientMaxBodySize);
		void setIndex(std::string index);
		void setErrorPageMap(int httpStatus, std::string path);
		void setLocation(LocationConfig location);

		const unsigned int			&getPort();
		const in_addr_t				&getHost();
		const std::string			&getServerName();
		const std::string			&getDocRoot();
		const unsigned int			&getClientMaxBodySize();
		const std::string			&getIndex();
		const std::map<int, std::string>	&getErrorPageMap();
		const std::vector<LocationConfig>	&getLocations();

};
#endif
