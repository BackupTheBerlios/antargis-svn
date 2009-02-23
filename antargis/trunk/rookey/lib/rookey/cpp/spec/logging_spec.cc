#include "rk_logging.h"

int main(int argc, char *argv[]) {
    RKLogging::getInstance()->clear();
    RKLogging::getInstance()->log("muh");

    assert(RKLogging::getInstance()->getLog() == "muh");
    return 0;
}