#ifndef RESOURCE_H
#define RESOURCE_H

#include <map>
#include <ag_xml.h>



/**
   Resource manages resources with BoA. Every entity has some kind of "bag", where
   everything is stored. This is it. Resources are identified by strings.
   You can get,set,sub and add resources.

   
FIXME: maybe remove Resource from c++??

*/
class Resource
{
  std::map<AGString,float> r;
 public:
  Resource();

  void saveXML(Node &node) const;
  void loadXML(const Node &node);

  float get(const AGString &pName);
  void add(const AGString &pName,float value);
  void sub(const AGString &pName,float value);
  void set(const AGString &pName,float value);

  std::map<AGString,float> getAll() const;

  /// take everything, that's in r and put it into "this"
  void takeAll(Resource &r);

  /// take everything of type pname, that's in r and put it into "this"
  void take(Resource &r,const AGString &pName);

  /// check if the "bag" is empty
  bool empty() const;
};


#endif
