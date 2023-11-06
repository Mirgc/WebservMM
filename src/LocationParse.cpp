#include "LocationParse.hpp"

LocationParse::LocationParse(void){

}


LocationParse::LocationParse(LocationParse const & src){
	*this = src;
}

LocationParse::~LocationParse(void){

}

LocationParse& LocationParse::operator=(const LocationParse &rhs){
	this->_ServerName = rhs.getServerName();
	this->_ProcessingLine = rhs.getProcessingLine();
	return (*this);
}

const std::string & LocationParse::getServerName(void) const{
	return(this->_ServerName);
}

const std::string & LocationParse::getProcessingLine(void) const{
	return(this->_ProcessingLine);
}

void LocationParse::setServerName(std::string const &ServerName){
 	this->_ServerName = ServerName;
}

void LocationParse::setProcessingLine(std::string const &ProcessingLine){
 	this->_ProcessingLine = ProcessingLine;
}

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
