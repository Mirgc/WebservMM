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

		// LocationConfig const & getLocationConfig();


		void getNextLocation(void);
		void getParsedLocations(void);

		std::string trim(const std::string &s);
		bool isStrInVector(const std::string &s, std::vector<std::string> const & vector);

		// Devolver un mapa o vector de pair de instancias de LocationConfig key:uploadName value: instancia de locartion
		// O tal vez un vector con las instancias directamente 

		// const std::vector<std::pair<std::string, std::string> > & getUploadCfg(void) const;
		// void setUploadCfg(std::pair<std::string, std::string> &pair);

};

// std::ostream & operator<<(std::ostream & o, LocationParse const & rhs);

#endif
