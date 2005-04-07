#include "ag_tools.h"
#include "ag_debug.h"

int toInt(const std::string &s)
{
  return atoi(s.c_str());
}
float toFloat(const std::string &s)
{
  float f=atof(s.c_str());
  cdebug(s<<":"<<f);
  return f;
}
