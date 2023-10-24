#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

#include <algorithm>
#include <iostream>
#include <vector>

class LocationConfig{

	private:
		std::string   	_name;
		std::vector<std::pair<std::string, std::string> > _cfg;

	public:
		LocationConfig(void);
		LocationConfig(LocationConfig const & src);
		~LocationConfig(void);

		LocationConfig & operator=(LocationConfig const & rhs);

		std::string const & getName(void) const;
		const std::vector<std::pair<std::string, std::string> > & getVector(void) const;

		void setName(std::string const &name);
		void setVector(std::pair<std::string, std::string> &pair);

};

std::ostream & operator<<(std::ostream & o, LocationConfig const & rhs);

#endif
