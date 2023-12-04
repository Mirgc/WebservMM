#include "LocationParse.hpp"

// Constructors
LocationParse::LocationParse(void){

}


LocationParse::LocationParse(LocationParse const & src){
	*this = src;
}

LocationParse::~LocationParse(void){
// 	if(!this->_ParsedLocations.empty())
// 		for(std::vector<LocationConfig*>::const_iterator it = this._ParsedLocations.begin();
		// it != this->_ParsedLocations.end(); ++it)
		// 	delete (*it);
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

const std::vector<LocationConfig>  & LocationParse::getParsedLocations(void) const{
	return(this->_ParsedLocations);
}

// Setters
void LocationParse::setServerName(std::string const &ServerName){
 	this->_ServerName = ServerName;
}

void LocationParse::setParsedLocations(std::vector<std::string> const &_ProcesingLocation){
 	this->_ProcesingLocation = _ProcesingLocation;
}

void LocationParse::setParsedLocations(std::vector<LocationConfig> const &ParsedLocations){
 	this->_ParsedLocations = ParsedLocations;
}

void LocationParse::addParsedLocations(std::string const &ProcesingLocation){
 	this->_ProcesingLocation.push_back(ProcesingLocation);
}

void LocationParse::addParsedLocations(LocationConfig const &ParsedLocation){
 	this->_ParsedLocations.push_back(ParsedLocation);
}

void LocationParse::setServerCfg(std::vector<std::string> const & serverCfg){
 	this->_serverConfig = serverCfg;	
}

// Class Methods
// Get all locations form ConfigFileParser class _serverConfig to process it
void LocationParse::getNextLocation(void){
	std::map<int, std::vector<std::string> >::iterator init;
	std::map<int, std::vector<std::string> >::iterator final;
	std::vector<std::string>::iterator start;
	std::vector<std::string>::iterator end;
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator itend;
	
	final = this->_eachServer.end();

	for(init =	this->_eachServer.begin(); init != final; init++){
		
		end = (*init).second.end();
		// ServerName Parse to identify instance and possible ServerName exceptions

		// !!! replantear como encontrar en el mapa el nombre

		for(start = (*init).second.begin(); start != end; start++){
			if((*start).find("server_name") != std::string::npos){
				// it = std::find(start, end, "server_name");
				// std::cout << "FULL CFG >> " << this->_serverConfig << std::endl;
				// std::cout << "end >> " << *end << std::endl;
				const std::string WHITESPACE = " \n\r\t\f\v";
				// if (it!=end)
				this->_ServerName = trim((*start).substr((*start).find("server_name")+11, std::string::npos));
				if (this->_ServerName.empty() or this->_ServerName.find_first_of(WHITESPACE) != std::string::npos)
						throw ParseException("Invalid Server Name");

			}
			// Extract location scopes from ConfigFileParser class to processing vector
			if((*start).find("location") != std::string::npos){
				// while(start != end){
					// it = std::find(start, end, "location");
					itend = std::find(start, end, "}");
					if (start != itend){
						while(start != itend){
							this->_ProcesingLocation.push_back(*start);
							start++;
						}
						this->_ProcesingLocation.push_back("}");
					// }
					// start = itend;
				}
			}
		}

		// Location and scopes basic verification
		// start =	this->_ProcesingLocation.begin();
		// end = this->_ProcesingLocation.end();

		// if (this->_ProcesingLocation.empty() or
		// (std::count(start, end, "{") - (std::count(start, end, "}")) != 0))
		// 		throw ParseException("No locations at file or bad scopes");
		// else{
		// 	this->addParsedLocations();
		// }
	}
	// if delete extracted data on src has sense (DISCUSS!!!!!!!!!)
	// start =	this->_serverConfig.begin();
	// it = std::find(start, itend, "location");
	// this->_serverConfig.erase(it, itend);
}

// Process RAW data vector and return a new vector of pointers to LocationConfig instances
void LocationParse::addParsedLocations(void){
	std::vector<std::string>::iterator start;
	std::vector<std::string>::iterator end;
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator itend;

	// This list of valid CFG keys may increase as needed and we should discuss a possible value list.
	std::string  Keys[] = { "proxy_pass", "method", "upload_enable", "upload_path",
							"redirection", "docroot", "autoindex", "index" };
	std::vector<std::string>  filledVector(Keys, Keys + sizeof(Keys)/sizeof(Keys[0]) );
	// std::vector<std::string>  validKeys = fillInVector(Keys);
	std::string key;
	std::string value;

	start =	this->_ProcesingLocation.begin();
	end = this->_ProcesingLocation.end();

	const std::string WHITESPACE = " }\n\r\t\f\v{";
	while (start != end)
	{
		// itend = std::find(start, end, "}");
		// if((trim(*start)).find("location")){
		LocationConfig loc = LocationConfig();
		loc.setUploadPath(trim((*start).substr((*start).find("location")+8, std::string::npos)));
		start++;
		itend = std::find(start, end, "}");
		while (start != itend){
			key = (*start).substr(trim(*start).find_first_not_of(WHITESPACE), trim(*start).find_first_of(WHITESPACE));
			value = trim((*start).substr(trim(*start).find_last_of(WHITESPACE), trim(*start).find_last_not_of(WHITESPACE)));
			if(isStrInVector(key, filledVector))
				loc.setUploadCfg(std::make_pair(key, value));
			else
				throw ParseException("Syntax error near " + key);
			start++;
		}
		this->addParsedLocations(loc);
		// }
		start++;
	}

}

// Validation supporting methods
// std::vector<std::string> LocationParse::fillInVector(std::string *Keys){

// 	// std::string *end = std::find(Keys, Keys + std::numeric_limits<size_t>::max(), "");
// 	std::vector<std::string>  filledVector(Keys, Keys + sizeof(Keys)/sizeof(Keys[0]) );
// 	return (filledVector);
// }

bool LocationParse::isStrInVector(const std::string &s, std::vector<std::string> const & v){
	
	if(std::find(v.begin(), v.end(), s) != v.end())
		return (true);
	return false;
}

// is a valid url format?
bool LocationParse::isUrlFormat(const std::string str){
	if (!str.empty()){
		std::string http = "http://";
		std::string https = "https://";
		size_t pos;
		
		pos = 0;
		if((pos = str.find_first_not_of(http)) == 7 or
		   (pos = str.find_first_not_of(https)) == 8){
			for(size_t it=0; it<pos; it++)
				if((pos == 7 and str[it] != http[it]) or
				   (pos == 8 and str[it] != https[it]))
					return false;
			for(size_t it=pos; isprint(str[it]) and !isspace(str[it]); it++)
				if (it == (str.length()-1))
					return true;
		}
	}
	return false;
}

bool LocationParse::isValidPath(const std::string str){
  std::ifstream ifs(str);

  if (ifs.is_open()) {
	ifs.close();
	return true;
  }

	throw ParseException("Invalid location path " + str);
	return false;
}

// BAD METHOD !!!!!!!!??????
std::vector<std::string> splitWords(const std::string &s) {
    std::stringstream ss(s);
    std::vector<std::string> words;
    std::string word;

    while (ss >> word)
        words.push_back(word);

    return words;
}
