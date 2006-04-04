#include "resource.h"


// RESOURCE

Resource::Resource()
{
}
float Resource::get(const std::string &pName)
{
  return r[pName];
}
void Resource::add(const std::string &pName,float value)
{
  r[pName]+=value;
}
void Resource::sub(const std::string &pName,float value)
{
  r[pName]=std::max(r[pName]-value,0.0f);
}
void Resource::set(const std::string &pName,float value)
{
  r[pName]=value;
}

void Resource::takeAll(Resource &pr)
{
  std::map<std::string,float>::iterator i=pr.r.begin();
  for(;i!=pr.r.end();i++)
    add(i->first,i->second);
  pr.r.clear();
}

std::map<std::string,float> Resource::getAll() const
{
  return r;
}


void Resource::saveXML(xmlpp::Node &node) const
{
  for(std::map<std::string,float>::const_iterator i=r.begin();i!=r.end();++i)
    node.set(i->first,toString(i->second));
  
}
void Resource::loadXML(const xmlpp::Node &node)
{
  xmlpp::Node::Attributes a=node.getAttributes();
  for(xmlpp::Node::Attributes::iterator i=a.begin();i!=a.end();++i)
    r[i->first]=toFloat(i->second);
}

bool Resource::empty() const
{
  for(std::map<std::string,float>::const_iterator i=r.begin();i!=r.end();++i)
    if(i->second>0)
      return false;
  return true;
}
