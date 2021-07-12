#ifndef TIMEUTIL_HPP
#define TIMEUTIL_HPP

#include "../inc/common.hpp"

class TimeUtil {
private:
	sf::Clock clock;
    sf::Time dt;
	float dtAsSeconds;
public:
	TimeUtil();
	~TimeUtil();

	void setTime();
	float getTime();
};

#endif
