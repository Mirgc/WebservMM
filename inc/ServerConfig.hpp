#ifndef SERVERCONFIG
# define SERVERCONFIG

# include <map>
# include <string>
# include <vector>
# include <arpa/inet.h>
# include "LocationConfig.hpp"

class ServerConfig{
	private:
		std::vector<unsigned int>	_listendPorts;
		unsigned int				_port;
		in_addr_t					_host;
		std::string					_serverName;
		std::string					_docRoot;
		unsigned int				_clientMaxBodySize;
		std::string					_index;
		std::map<int, std::string>	_errorPageMap;
		std::vector<LocationConfig>	_locations;

	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig(const ServerConfig &copy);
		ServerConfig &operator=(const ServerConfig &obj);

	void setPort(unsigned int const &port);
	void setHost(in_addr_t const &host);
	void setServerName(std::string const &_serverName);
	void setDocRoot(std::string const &root);
	void setClientMaxBodySize(unsigned int const &clientMaxBodySize);
	void setIndex(std::string const &index);
	void setErrorPageMap(int const &httpStatus, std::string const &path);
	void setLocation(std::vector<LocationConfig> const &location);
	void addLocation(LocationConfig location);
	void setListenedPorts(std::vector<unsigned int> ports);
	void addListenedPorts(unsigned int port);

	const unsigned int &getPortAt(unsigned int pos) const;
	const unsigned int &getPort() const;
	const in_addr_t &getHost() const;
	const std::string &getServerName() const;
	const std::string &getDocRoot() const;
	unsigned int getClientMaxBodySize() const;
	const std::string &getIndex() const;
	const std::map<int, std::string> &getErrorPageMap() const;
	const std::vector<LocationConfig> &getLocations() const;
	const std::vector<unsigned int> &getListenPorts() const;

};
#endif
