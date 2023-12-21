#ifndef LOCATION_PARSE_HPP
# define LOCATION_PARSE_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include "ConfigFileParser.hpp"
#include "LocationConfig.hpp"
#include "ServerConfig.hpp"

class Parse: public ConfigFileParser{

	protected:
		std::string   		     		_ServerName;
		std::vector<std::string>    	_ProcesingLocation;
		std::vector<LocationConfig>  	_ParsedLocations;
		std::vector<ServerConfig>  		_ParsedCfgs;

	public:
		Parse(void);
		Parse(Parse const & src);
		~Parse(void);

		Parse & operator=(Parse const & rhs);

		std::string const & getServerName(void) const;
		std::vector<std::string>  const & getProcesingLocation(void) const;
		// get full vector
		std::vector<LocationConfig>  const & getParsedLocations(void) const;
		const std::vector<ServerConfig> & getParsedCfgs(void) const;
		const ServerConfig & getParsedCfgAt(const unsigned int pos) const;

		void setServerName(std::string const &ServerName);
		void setProcesingLocations(std::vector<std::string> const & ProcesingLocation);
		void setParsedLocations(std::vector<LocationConfig>  const & ParsedLocation);
		void setServerCfg(std::vector<std::string> const & serverCfg);
		void setFullCfg(std::string const & configFile);
		
		// add one to vector
		void addProcesingLocations(std::string const & ProcesingLocation);
		void addParsedLocations(LocationConfig const & ParsedLocation);

		// Get all server config to process it
		void getNextServer(void);
		// Process RAW data vector and add a copy from LocationConfig filled instance 
		void ParseLocations(ServerConfig srvCfg);
		

		// check if specific string is in vector
		bool isStrInVector(const std::string &s, std::vector<std::string> const & vector);

		// is a valid url format?
		bool isUrlFormat(const std::string str);

		// is a valid path?
		bool isValidPath(const std::string str);

		std::vector<unsigned int> splitPorts(const std::string &s);

		in_addr_t strToIp(const std::string ipString);

		bool isDigitStr(std::string str);

		// return a new sting vector contructed from an array
		// std::vector<std::string> fillInVector(std::string *Keys);
};

#endif
