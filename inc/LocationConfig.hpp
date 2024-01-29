#ifndef LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP

#include <algorithm>
#include <iostream>
#include <vector>

class LocationConfig{

	private:
		std::string   	_UploadPath;
		std::vector<std::pair<std::string, std::string> > _UploadCfg;

	public:
		LocationConfig(void);
		LocationConfig(LocationConfig const & src);
		~LocationConfig(void);

		LocationConfig & operator=(LocationConfig const & rhs);

		std::string const & getUploadPath(void) const;
		const std::vector<std::pair<std::string, std::string> > & getUploadCfg(void) const;

		// get specific fg value by name 
		const std::string & getCfgValueFrom(std::string const & Key) const;
		// specific string getters
		const std::string getProxyPass() const;
		const std::string getRedirection() const;
		const std::string getDocroot() const;
		const std::string getIndex() const;
		// bool getters
		bool getAutoIndexBool() const;
		bool getUploadEnableBool() const;
		bool getMethodGetBool() const;
		bool getMethodPostBool() const;
		bool getMethodDeleteBool() const;
		// check if specific key is in location
		bool isKeyInLocation(std::string const & Key) const;
		// specific method validation
		bool isMethodInLocation(std::string verb) const;

		void setUploadPath(std::string const &UploadPath);
		void setUploadCfg(std::pair<std::string, std::string> const &pair);

};

std::ostream & operator<<(std::ostream & o, LocationConfig const & rhs);

#endif
