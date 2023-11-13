#ifndef LOCATION_PARSE_HPP
# define LOCATION_PARSE_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include "ConfigFileParser.hpp"
#include "LocationConfig.hpp"

class LocationParse: public ConfigFileParser{

	private:
		std::string   		     		_ServerName;
		std::vector<std::string>    	_ProcesingLocation;
		std::vector<LocationConfig*>  	_ParsedLocations;

	public:
		LocationParse(void);
		LocationParse(LocationParse const & src);
		~LocationParse(void);

		LocationParse & operator=(LocationParse const & rhs);

		std::string const & getServerName(void) const;
		std::vector<std::string>  const & getProcesingLocation(void) const;
		std::vector<LocationConfig*>  const & getParsedLocations(void) const;

		void setServerName(std::string const &ServerName);
		void setParsedLocations(std::vector<std::string> const &_ProcesingLocation);
		void setParsedLocations(std::vector<LocationConfig*>  const &_ParsedLocations);

		// Get all locations form ConfigFileParser class _serverConfig to process it
		void getNextLocation(void);
		// Process RAW data vector and return a new vector of pointers to LocationConfig instances
		void getParsedLocations(void);
		
		// trim " }\n\r\t\f\v{" from string
		std::string trim(const std::string &s);

		// check if specific string is in vector
		bool isStrInVector(const std::string &s, std::vector<std::string> const & vector);
		
		// return a new sting vector contructed from an array
		std::vector<std::string> fillInVector(std::string *Keys);
};

#endif
