#ifndef ANT_CONFIG_H
#define ANT_CONFIG_H

#include <map>
#include <string>

class AntConfig
{
 public:
  AntConfig();

  std::string get(const std::string &pValue) const;
  void set(const std::string &pName,const std::string &pValue);

 private:
  void writeToDisc();

  std::map<std::string,std::string> singleValue;
};

AntConfig *getConfig();

#endif
