#ifndef LOCATION_PARSE_HPP
# define LOCATION_PARSE_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include "ConfigFileParser.hpp"
#include "LocationConfig.hpp"

class LocationParse: public ConfigFileParser{

	protected:
		std::string   		     		_ServerName;
		std::vector<std::string>    	_ProcesingLocation;
		std::vector<LocationConfig>  	_ParsedLocations;

	public:
		LocationParse(void);
		LocationParse(LocationParse const & src);
		~LocationParse(void);

		LocationParse & operator=(LocationParse const & rhs);

		std::string const & getServerName(void) const;
		std::vector<std::string>  const & getProcesingLocation(void) const;
		// get full vector
		std::vector<LocationConfig>  const & getParsedLocations(void) const;

		void setServerName(std::string const &ServerName);
		void setParsedLocations(std::vector<std::string> const & ProcesingLocation);
		void setParsedLocations(std::vector<LocationConfig>  const & ParsedLocation);
		void setServerCfg(std::vector<std::string> const & serverCfg);
		
		// add one to vector
		void addParsedLocations(std::string const & ProcesingLocation);
		void addParsedLocations(LocationConfig const & ParsedLocation);

		// Get all locations form ConfigFileParser class _serverConfig to process it
		void getNextLocation(void);
		// Process RAW data vector and add a copy from LocationConfig filled instance 
		void addParsedLocations(void);
		

		// check if specific string is in vector
		bool isStrInVector(const std::string &s, std::vector<std::string> const & vector);

		// is a valid url format?
		bool isUrlFormat(const std::string str);

		// is a valid path?
		bool isValidPath(const std::string str);
		
		// return a new sting vector contructed from an array
		// std::vector<std::string> fillInVector(std::string *Keys);
};

#endif
