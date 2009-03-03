#include "rk_logging.h"

void testClear() {
  RKLogging::getInstance()->clear();
  RKLogging::getInstance()->logDirect("muh");

  assert(RKLogging::getInstance()->getLog() == "muh");

}

void testEasyLog()
{
  logger::err<<"myMUH"<<logger::endl;

  
}

int main(int argc, char *argv[]) {
  testClear();

  testEasyLog();

  return 0;
}