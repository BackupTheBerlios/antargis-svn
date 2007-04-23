#include "ag_config.h"
#include "ag_xml.h"

AGConfig::AGConfig()
{
  CTRACE;
  Document doc("config.xml");

  Node &root=doc.root();
  AGString comment;
  for(Node::iterator i=root.begin();i!=root.end();i++)
    {
      if((*i)->get("name")=="")
	{
	  AGString c=(*i)->getContent();
	  if(c.substr(0,4)=="<!--")
	    {
	      comment=c;
	      //	      cdebug("comment:"<<comment);
	    }
	  else
	    {
	      //	      cdebug("no comment."<<c);
	    }
	}
      else
	{
	  singleValue[(*i)->get("name")]=(*i)->get("value");
	  comments[(*i)->get("name")]=comment;
	  cdebug((*i)->get("name")<<":"<<(*i)->get("value")<<":"<<comment);
	  comment="";
	}
    }

  //  cdebug("singleValues:"<<singleValue.size());

  //  std::cerr<<doc.toString()<<std::endl;

  //  writeToDisc();
}

AGString AGConfig::get(const AGString &pValue,const AGString &pDefault,const AGString &pComment)
{
  AGString v=get(pValue);
  if(v=="")
    {
      comments[pValue]=pComment;
      set(pValue,pDefault);
      v=pDefault;
    }
  return v;
}



AGString AGConfig::get(const AGString &pValue) const
{
  std::map<AGString,AGString>::const_iterator i=singleValue.find(pValue);
  if(i==singleValue.end())
    return "";
  else
    return i->second;
}

void AGConfig::set(const AGString &pName,const AGString &pValue)
{
  singleValue[pName]=pValue;
  writeToDisc();
}

void AGConfig::writeToDisc()
{
  Document doc;
  Node &root=doc.root();
  root.setName("config");
  for(std::map<AGString,AGString>::const_iterator i=singleValue.begin();i!=singleValue.end();i++)
    {
      if(comments[i->first]!="")
	{
	  Node &n=root.addChild("");
	  n.setContent(AGString("\n")+comments[i->first]+"\n");
	}

      Node &n=root.addChild("option");
      n.set("name",i->first);
      n.set("value",i->second);
    }

  saveFile("config.xml",doc.toString(true));
}



AGConfig *gConfig=0;
AGConfig *getConfig()
{
  if(!gConfig)
    gConfig=new AGConfig;
  return gConfig;
}

void updateConfig()
{
  delete gConfig;
  gConfig=0;
  //  getConfig();
}