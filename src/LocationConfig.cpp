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

/* print_location_method pending */
