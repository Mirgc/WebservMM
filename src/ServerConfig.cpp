#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
        this->_port = 0;
        this->_host = 0;
        this->_serverName = "";
        this->_docRoot = "";
        this->_clientMaxBodySize = 0;
        this->_index = "";
}

ServerConfig::~ServerConfig() {}

/* copy constructor */
ServerConfig::ServerConfig(const ServerConfig &copy)
{
        if (this != &copy)
        {
                this->_port = copy._port;
                this->_host = copy._host;
                this->_serverName = copy._serverName;
                this->_docRoot = copy._docRoot;
                this->_clientMaxBodySize= copy._clientMaxBodySize;
                this->_index = copy._index;
                this->_errorPageMap = copy._errorPageMap;
                this->_locations = copy._locations;

        }
        return ;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &obj)
{
        if (this != &obj)
        {
                this->_port = obj._port;
                this->_host = obj._host;
                this->_serverName = obj._serverName;
                this->_docRoot = obj._docRoot;
                this->_clientMaxBodySize = obj._clientMaxBodySize;
                this->_index = obj._index;
                this->_errorPageMap = obj._errorPageMap;
                this->_locations = obj._locations;
        }
        return (*this);
}

void ServerConfig::addListenedPorts(unsigned int port){
        this->_listendPorts.push_back(port);
}

void ServerConfig::setListenedPorts(std::vector<unsigned int> ports){
        this->_listendPorts = ports;
}

void ServerConfig::setPort(unsigned int const &port){
	this->_port = port;
}

void ServerConfig::setHost(in_addr_t const &host){
	this->_host = host;
}

void ServerConfig::setServerName(std::string const &serverName){
	this->_serverName= serverName;
}

void ServerConfig::setDocRoot(std::string const &docRoot){
	this->_docRoot = docRoot;
}

void ServerConfig::setClientMaxBodySize(unsigned int const &clientMaxBodySize){
	this->_clientMaxBodySize = clientMaxBodySize;
}

void ServerConfig::setIndex(std::string const &index){
	this->_index = index;
}

void ServerConfig::setErrorPageMap(int const &httpStatus, std::string const &path){
	this->_errorPageMap.insert(std::pair<int,std::string>(httpStatus,path));
}

void ServerConfig::addLocation(LocationConfig location){
	this->_locations.insert(_locations.end(), location);
}

void ServerConfig::setLocation(std::vector<LocationConfig> const &locations){
	this->_locations = locations;
}

const unsigned int &ServerConfig::getPortAt(unsigned int pos) const{
	return (this->_listendPorts.at(pos));
}

const unsigned int &ServerConfig::getPort() const{
	return (this->_port);
}
 
const in_addr_t	&ServerConfig::getHost() const{
	return (this->_host);
}

const std::string &ServerConfig::getServerName() const{
	return (this->_serverName);
}

const std::string &ServerConfig::getDocRoot() const{
	return (this->_docRoot);
}

unsigned int ServerConfig::getClientMaxBodySize() const{
	return (this->_clientMaxBodySize);
}
	
const std::string &ServerConfig::getIndex() const{
	return (this->_index);
}

const std::map<int, std::string> &ServerConfig::getErrorPageMap() const{
	return (this->_errorPageMap);
}

const std::vector<LocationConfig> &ServerConfig::getLocations() const{
	return (this->_locations);
}
