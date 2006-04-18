#include "ant_config.h"
#include "ag_xml.h"

AntConfig::AntConfig()
{
  Document doc("config.xml");

  Node &root=doc.root();
  for(Node::iterator i=root.begin();i!=root.end();i++)
    {
      singleValue[(*i)->get("name")]=(*i)->get("value");
      cdebug((*i)->get("name")<<":"<<(*i)->get("value"));
    }

  cdebug("singleValues:"<<singleValue.size());
}

std::string AntConfig::get(const std::string &pValue) const
{
  std::map<std::string,std::string>::const_iterator i=singleValue.find(pValue);
  if(i==singleValue.end())
    return "";
  else
    return i->second;
}

void AntConfig::set(const std::string &pName,const std::string &pValue)
{
  singleValue[pName]=pValue;
  writeToDisc();
}

void AntConfig::writeToDisc()
{
  Document doc;
  Node &root=doc.root();
  root.setName("config");
  for(std::map<std::string,std::string>::const_iterator i=singleValue.begin();i!=singleValue.end();i++)
    {
      Node &n=root.addChild("option");
      n.set("name",i->first);
      n.set("value",i->second);
    }

  saveFile("config.xml",doc.toString());
}



AntConfig *gConfig=0;
AntConfig *getConfig()
{
  if(!gConfig)
    gConfig=new AntConfig;
  return gConfig;
}
