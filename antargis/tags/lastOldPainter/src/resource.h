#ifndef RESOURCE_H
#define RESOURCE_H


// FIXME: maybe remove Resource from c++??
class Resource
{
  std::map<std::string,float> r;
 public:
  Resource();
  float get(const std::string &pName);
  void add(const std::string &pName,float value);
  void sub(const std::string &pName,float value);
  void set(const std::string &pName,float value);

  std::map<std::string,float> getAll() const;

  void takeAll(Resource &r);
};


#endif
