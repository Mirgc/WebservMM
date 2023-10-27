#include "LocationConfig.hpp"

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

const std::string & LocationConfig::getUploadPath(void) const{
	return(this->_UploadPath);
}

const std::vector<std::pair<std::string, std::string> > & LocationConfig::getUploadCfg(void) const{
	return(this->_UploadCfg);
}

void LocationConfig::setUploadPath(std::string const &UploadPath){
	this->_UploadPath = UploadPath;
}

void LocationConfig::setUploadCfg(std::pair<std::string, std::string> &pair){
	this->_UploadCfg.push_back(pair);
}

std::ostream & operator<<(std::ostream & o, LocationConfig const & rhs){
	std::cout << "Location" << " : " << rhs.getUploadPath() << std::endl;
	std::cout << "KEY" << " : " << "VALUE" << std::endl;
	for(std::vector<std::pair<std::string, std::string> >::const_iterator it = rhs.getUploadCfg().begin();
	it != rhs.getUploadCfg().end(); ++it)
		std::cout << (*it).first << " : " << (*it).second << std::endl;
	return (o);
}
