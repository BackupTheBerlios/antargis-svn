
#include "rk_logging.h"

#include <sstream>
#include <iostream>


template
class RKSingleton<RKLogging>;

RKLogging::RKLogging() : output("rk_log.txt",std::ios_base::app) {
}

RKLogging::~RKLogging() {
    output.flush();
    output.close();
}

void RKLogging::log(const std::string &s) {
    output << s;
    output.flush();
}

void RKLogging::clear()
{
    output.close();
    output.open("rk_log.txt");
}


std::string RKLogging::getLog()  {
    output.flush();

    std::ifstream ifs("rk_log.txt");
    std::ostringstream os;
    std::string buf;

    while (!ifs.eof()) {
        ifs>>buf;
        os << buf;
    }
    std::cout<<"OS:"<<os.str()<<std::endl;
    return os.str();

}
