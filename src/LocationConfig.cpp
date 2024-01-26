#include "LocationConfig.hpp"

// Constructors
LocationConfig::LocationConfig(void){

}


LocationConfig::LocationConfig(LocationConfig const & src){
	*this = src;
}

LocationConfig::~LocationConfig(void){

}

LocationConfig& LocationConfig::operator=(const LocationConfig &rhs){
	this->_UploadPath = rhs.getUploadPath();
	this->_UploadCfg = rhs.getUploadCfg();
	return (*this);
}

// get location Upload Path
const std::string & LocationConfig::getUploadPath(void) const{
	return(this->_UploadPath);
}

bool LocationConfig::isPyCgi() const {
	if(this->getUploadPath().compare(".py") == 0)
		return true;
	return false;
}

// get all cfg vector<pair> key.first value.second
const std::vector<std::pair<std::string, std::string> > & LocationConfig::getUploadCfg(void) const{
	return(this->_UploadCfg);
}

// get specific cfg value by name 
const std::string & LocationConfig::getCfgValueFrom(std::string const & Key) const{
	for(std::vector<std::pair<std::string, std::string> >::const_iterator it = this->getUploadCfg().begin();
		it != this->getUploadCfg().end(); ++it){
		if ((*it).first == Key)
			return ((*it).second);
	}
	return (Key);
}

// check if specific key is in location
bool LocationConfig::isKeyInLocation(std::string const & Key) const{
	for(std::vector<std::pair<std::string, std::string> >::const_iterator it = this->getUploadCfg().begin();
		it != this->getUploadCfg().end(); ++it){
			if ((*it).first == Key)
				return true;
		}
	return false;
}

// setters
void LocationConfig::setUploadPath(std::string const &UploadPath){
	this->_UploadPath = UploadPath;
}

void LocationConfig::setUploadCfg(std::pair<std::string, std::string> const &pair){
	this->_UploadCfg.push_back(pair);
}

// print all cfg vector<pair> key.first value.second if needed to debug
std::ostream & operator<<(std::ostream & o, LocationConfig const & rhs){
	std::cout << "Location" << " : " << rhs.getUploadPath() << std::endl;
	std::cout << "KEY" << " : " << "VALUE" << std::endl;
	for(std::vector<std::pair<std::string, std::string> >::const_iterator it = rhs.getUploadCfg().begin();
	it != rhs.getUploadCfg().end(); ++it)
		std::cout << (*it).first << " : " << (*it).second << std::endl;
	return (o);
}

// specific method validation
bool LocationConfig::isMethodInLocation(std::string verb) const{
	for(std::vector<std::pair<std::string, std::string> >::const_iterator it = this->getUploadCfg().begin();
		it != this->getUploadCfg().end(); ++it){
			if ((*it).first == "method" and (*it).second == verb)
				return true;
		}
	return false;
}
