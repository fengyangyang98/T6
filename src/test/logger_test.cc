#include "core.hpp"
#include "logger.hpp"

void loggerTest()
{
    Log l(1, LOG_PRE, "nihao");
    std::string sr = l.logToStr();

    Log k(sr);
    std::cout << k.ID << std::endl;
    std::cout << k.state << std::endl;
    std::cout << k.event.length() << " " << k.event << std::endl;
}