#ifndef LOCATION_PARSE_HPP
# define LOCATION_PARSE_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include "ConfigFileParser.hpp"
#include "LocationConfig.hpp"

class LocationParse: public ConfigFileParser{

	private:
		std::string   	_ServerName;
		std::string   	_ProcessingLine;
		// std::vector<std::pair<std::string, std::string> > _UploadCfg;

	public:
		LocationParse(void);
		LocationParse(LocationParse const & src);
		~LocationParse(void);

		LocationParse & operator=(LocationParse const & rhs);

		std::string const & getServerName(void) const;
		std::string const & getProcessingLine(void) const;

		void setServerName(std::string const &ServerName);
		void setProcessingLine(std::string const &PorcessingLine);

		
		// Devolver un mapa o vector de pair de instancias de LocationConfig key:uploadName value: instancia de locartion
		// O tal vez un vector con las instancias directamente 

		// const std::vector<std::pair<std::string, std::string> > & getUploadCfg(void) const;
		// void setUploadCfg(std::pair<std::string, std::string> &pair);

};

// std::ostream & operator<<(std::ostream & o, LocationParse const & rhs);

#endif
