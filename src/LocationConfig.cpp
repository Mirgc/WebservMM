#include "LocationConfig.hpp"

LocationConfig::LocationConfig(void){

}


LocationConfig::LocationConfig(LocationConfig const & src){
	*this = src;
}

LocationConfig::~LocationConfig(void){

}

LocationConfig& LocationConfig::operator=(const LocationConfig &rhs){
	this->_name = rhs.getName();
	this->_cfg = rhs.getVector();
	return (*this);
}

const std::string & LocationConfig::getName(void) const{
	return(this->_name);
}

const std::vector<std::pair<std::string, std::string> > & LocationConfig::getVector(void) const{
	return(this->_cfg);
}

void LocationConfig::setName(std::string const &name){
	this->_name = name;
}

void LocationConfig::setVector(std::pair<std::string, std::string> &pair){
	this->_cfg.push_back(pair);
}

std::ostream & operator<<(std::ostream & o, LocationConfig const & rhs){
	std::cout << "Location" << " : " << rhs.getName() << std::endl;
	std::cout << "KEY" << " : " << "VALUE" << std::endl;
	for(std::vector<std::pair<std::string, std::string> >::const_iterator it = rhs.getVector().begin();
	it != rhs.getVector().end(); ++it)
		std::cout << (*it).first << " : " << (*it).second << std::endl;
	return (o);
}
