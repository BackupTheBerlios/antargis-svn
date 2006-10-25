#include "resource.h"


// RESOURCE

Resource::Resource()
{
}
float Resource::get(const AGString &pName)
{
  return r[pName];
}
void Resource::add(const AGString &pName,float value)
{
  r[pName]+=value;
}
void Resource::sub(const AGString &pName,float value)
{
  r[pName]=std::max(r[pName]-value,0.0f);
}
void Resource::set(const AGString &pName,float value)
{
  r[pName]=value;
}

void Resource::takeAll(Resource &pr)
{
  std::map<AGString,float>::iterator i=pr.r.begin();
  for(;i!=pr.r.end();i++)
    add(i->first,i->second);
  pr.r.clear();
}

void Resource::take(Resource &r,const AGString &pName)
{
  add(pName,r.get(pName));
  r.set(pName,0);
}


std::map<AGString,float> Resource::getAll() const
{
  return r;
}


void Resource::saveXML(Node &node) const
{
  for(std::map<AGString,float>::const_iterator i=r.begin();i!=r.end();++i)
    node.set(i->first,AGString(i->second));
  
}
void Resource::loadXML(const Node &node)
{
  Node::Attributes a=node.getAttributes();
  for(Node::Attributes::iterator i=a.begin();i!=a.end();++i)
    r[i->first]=i->second.toFloat();
}

bool Resource::empty() const
{
  for(std::map<AGString,float>::const_iterator i=r.begin();i!=r.end();++i)
    if(i->second>0)
      return false;
  return true;
}
