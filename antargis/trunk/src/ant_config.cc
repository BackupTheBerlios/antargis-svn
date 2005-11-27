#include "ant_config.h"
#include "ag_xml.h"

AntConfig::AntConfig()
{
  Document doc("config.xml");

  Node &root=doc.root();
  for(Node::iterator i=root.begin();i!=root.end();i++)
    {
      singleValue[(*i)->get("name")]=(*i)->get("value");
    }
}

std::string AntConfig::get(const std::string &pValue) const
{
  std::map<std::string,std::string>::const_iterator i=singleValue.find(pValue);
  if(i==singleValue.end())
    return "";
  else
    return i->second;
}



AntConfig *gConfig=0;
AntConfig *getConfig()
{
  if(!gConfig)
    gConfig=new AntConfig;
  return gConfig;
}
