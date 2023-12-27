#include "StringTools.hpp"
/* orthodox canonical class form requirement */

StringTools::StringTools(void) : _toTrim("0"), _string("0"){

}

StringTools::StringTools(const std::string toTrim, const std::string string) : _toTrim(toTrim), _string(string){

}


StringTools::StringTools(StringTools const & src){
	*this = src;
}

StringTools::~StringTools(void){

}

StringTools & StringTools::operator=(StringTools const & rhs){
	if (this != &rhs){
		this->_toTrim = rhs._toTrim;
		this->_string = rhs._string;
	}
	return(*this);
}

// Methods
std::string StringTools::ltrim(const std::string &s, std::string totrim){
    size_t start = s.find_first_not_of(totrim);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string StringTools::rtrim(const std::string &s, std::string totrim){
    size_t end = s.find_last_not_of(totrim);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string StringTools::trim(const std::string &s, std::string totrim) {
    return rtrim(ltrim(s, totrim), totrim);
}
