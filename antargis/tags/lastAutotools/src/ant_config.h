#ifndef ANT_CONFIG_H
#define ANT_CONFIG_H

#include <map>
#include <string>

class AntConfig
{
 public:
  AntConfig();

  std::string get(const std::string &pValue) const;

 private:
  std::map<std::string,std::string> singleValue;
};

AntConfig *getConfig();

#endif
