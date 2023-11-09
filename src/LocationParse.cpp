#include "LocationParse.hpp"

// Constructors
LocationParse::LocationParse(void){

}


LocationParse::LocationParse(LocationParse const & src){
	*this = src;
}

LocationParse::~LocationParse(void){

}

LocationParse& LocationParse::operator=(const LocationParse &rhs){
	this->_ServerName = rhs.getServerName();
	this->_ProcesingLocation = rhs.getProcesingLocation();
	this->_ParsedLocations = rhs.getParsedLocations();
	return (*this);
}

// Getters
const std::string & LocationParse::getServerName(void) const{
	return(this->_ServerName);
}

const std::vector<std::string> & LocationParse::getProcesingLocation(void) const{
	return(this->_ProcesingLocation);
}

const std::vector<LocationConfig*>  & LocationParse::getParsedLocations(void) const{
	return(this->_ParsedLocations);
}

// Setters
void LocationParse::setServerName(std::string const &ServerName){
 	this->_ServerName = ServerName;
}

void LocationParse::setParsedLocations(std::vector<std::string> const &_ProcesingLocation){
 	this->_ProcesingLocation = _ProcesingLocation;
}

void LocationParse::setParsedLocations(std::vector<LocationConfig*>  const &ParsedLocations){
 	this->_ParsedLocations = ParsedLocations;
}

// Class Methods
// Get all locations form ConfigFileParser class _serverConfig to process it
void LocationParse::getNextLocation(void){
	std::vector<std::string>::iterator start;
	std::vector<std::string>::iterator end;
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator itend;
	
	start =	this->_serverConfig.begin();
	end = this->_serverConfig.end();

	// ServerName Parse to identify instance and possible ServerName exceptions
	it = std::find(start, end, "server_name");
	const std::string WHITESPACE = " \n\r\t\f\v";
	if (it!=end)
		this->_ServerName = trim((*it).substr((*it).find("server_name")+11, std::string::npos));
	if (this->_ServerName.empty() or this->_ServerName.find_first_of(WHITESPACE) != std::string::npos)
			throw ParseException("Invalid Server Name");

	// Extract location scopes from ConfigFileParser class to processing vector
	while(start != end){
		it = std::find(start, end, "location");
		if (it != end){
			itend = std::find(it, end, "}");
			while(it != itend){
				this->_ProcesingLocation.push_back(*it);
				it++;
			}
		}
		start = itend;
	}

	// Location and scopes basic verification
	start =	this->_ProcesingLocation.begin();
	end = this->_ProcesingLocation.end();

	if (this->_ProcesingLocation.empty() or
	   (std::count(start, end, "{") - (std::count(start, end, "}")) != 0))
			throw ParseException("No locations at file");

	// if delete extracted data on src has sense (DISCUSS)
	start =	this->_serverConfig.begin();
	it = std::find(start, itend, "location");
	this->_serverConfig.erase(it, itend);
}

// Process RAW data vector and return a new vector of pointers to LocationConfig instances
void LocationParse::getParsedLocations(void){
	std::vector<std::string>::iterator start;
	std::vector<std::string>::iterator end;
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator itend;

	start =	this->_ProcesingLocation.begin();
	end = this->_ProcesingLocation.end();

	while (start != end)
	{
		if((*start).find("location")){
			LocationConfig* loc = new LocationConfig();
			loc->setUploadPath(trim((*start).substr((*start).find("location")+8, std::string::npos)));
			// continue here charging vector of pairs parsing _ProcesingLocation from each loc instance
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
		start++;
	}

}

// string trim method support functions (maybe move to Upper Class)
std::string ltrim(const std::string &s){
	const std::string WHITESPACE = " }\n\r\t\f\v{";
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s){
	const std::string WHITESPACE = " }\n\r\t\f\v{";
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string LocationParse::trim(const std::string &s) {
	return rtrim(ltrim(s));
}


	// for(start=this->_serverConfig.begin(); start!=this->_serverConfig.end(); start++){
	// 	it = std::find(start, end, "location");

	// 	if (it != end){
	// 		itend = std::find(start, end, "}");
	// 		for(it; it!=itend; it++)
	// 			this->_ProcesingLocation.push_back(*it);
	// 	}
	// }

// void LocationParse::setUploadCfg(std::pair<std::string, std::string> &pair){
// this->_UploadCfg.push_back(pair);
// }

// std::ostream & operator<<(std::ostream & o, LocationParse const & rhs){
//  	std::cout << "Location" << " : " << rhs.getUploadPath() << std::endl; 
//  	std::cout << "KEY" << " : " << "VALUE" << std::endl;
//  	for(std::vector<std::pair<std::string, std::string> >::const_iterator it = rhs.getUploadCfg().begin();
//  	it != rhs.getUploadCfg().end(); ++it)
// 		std::cout << (*it).first << " : " << (*it).second << std::endl;
// 	return (o);
// }
