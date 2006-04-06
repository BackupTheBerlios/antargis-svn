#ifndef RESOURCE_H
#define RESOURCE_H

#include <map>
#include <ag_xml.h>

// FIXME: maybe remove Resource from c++??
class Resource
{
  std::map<std::string,float> r;
 public:
  Resource();

  void saveXML(Node &node) const;
  void loadXML(const Node &node);


  float get(const std::string &pName);
  void add(const std::string &pName,float value);
  void sub(const std::string &pName,float value);
  void set(const std::string &pName,float value);

  std::map<std::string,float> getAll() const;

  void takeAll(Resource &r);
  void take(Resource &r,const std::string &pName);

  bool empty() const;
};


#endif
