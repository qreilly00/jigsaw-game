#include "../TimeUtil.hpp"

void TimeUtil::setTime() {
	dt = clock.restart();
    dtAsSeconds = dt.asMilliseconds();
	//dtAsSeconds = dt.asSeconds();
    //std::cout << dt.asMilliseconds() << std::endl;
}
