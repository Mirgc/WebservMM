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
	this->_vector = rhs.getVector();
	return (*this);
}

const unsigned int & LocationConfig::getName(void) const{
	return(this->_Name);
}

const std::vector<std::pair<string, string>> & LocationConfig::getVector(void) const{
	return(this->_vector);
}

void LocationConfig::setName(std::string const &name){
	this->_name = name;
}

void LocationConfig:setVector(std::pair<string, string> &pair){
	this->vector.push_back(pair);
}

std::ostream & LocationConfig::operator<<(std::ostream & o, LocationConfig const & rhs){
	std::cout << "Location" << " : " << this->_name << std::endl;
	std::cout << "KEY" << " : " << "VALUE" << std::endl;
	for(std::vector<std::pair<string, string>>::const_iterator it = this->_cfg.begin();
	it != this->_cfg.end(); ++it)
		std::cout << (*it).first() << " : " << (it*).second() << std::endl;
}
