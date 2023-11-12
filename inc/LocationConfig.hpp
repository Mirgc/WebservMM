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

		void setUploadPath(std::string const &UploadPath);
		void setUploadCfg(std::pair<std::string, std::string> const &pair);

};

std::ostream & operator<<(std::ostream & o, LocationConfig const & rhs);

#endif
