/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_xml.cc
 * by David Kamphausen (david.kamphausen@web.de)
 *
 * The "Antargis" project, including all files needed to compile it,
 * is free software; you can redistribute it and/or use it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 */

#include "ag_xml.h"
#include "ag_fs.h"
#include "ag_debug.h"

/****************************************************************
 * ParserException
 ****************************************************************/

ParserException::ParserException(std::string u):s(u)
{
  CTRACE;
  cdebug("u:"<<u);
}

std::string ParserException::what()
{
  return s;
}

/****************************************************************
 * ParserInfo
 ****************************************************************/

char ParserInfo::next()
{
  if(s.length()>p)
    return s[p];
  else
    return 0;
}
std::string ParserInfo::getNext2()
{
  if(s.length()>p+1)
    return s.substr(p,2);
  return "";
}
std::string ParserInfo::getNext3()
{
  if(s.length()>p+2)
    return s.substr(p,3);
  return "";
}
void ParserInfo::inc()
{
  if(s.length()>p)
    p++;
}
bool ParserInfo::end()
{
  return(p>=s.length());
}

std::string ParserInfo::getInfo()
{
  std::ostringstream os;
  os<<"Error beginning at '"<<s[p]<<"':"<<s.substr(p,std::min(s.length()-p,p+20));
  return os.str();
}

/****************************************************************
 * Node
 ****************************************************************/

Node::Node()
{}
Node::Node(std::string name):mName(name)
{}

void Node::setName(std::string pName)
{
  mName=pName;
}

Node::NodeList Node::get_children() const
  {
    return mNodes;
  }

const Node &Node::operator*() const
  {
    return *this;
  }
const Node *Node::operator->() const
  {
    return this;
  }

Node::NodeList Node::get_children(std::string pName) const
  {
    NodeList l;
    NodeList::const_iterator i=mNodes.begin();
    for(;i!=mNodes.end();i++)
      {
        if(i->getName()==pName)
          l.push_back(*i);
      }
    return l;
  }

std::string Node::getName() const
  {
    return mName;
  }
std::string Node::get_name() const
  {
    return mName;
  }

Node &Node::newChild(std::string pName)
{
  mNodes.push_back(Node(pName));
  return mNodes.back();
}

Node *Node::add_child(std::string n)
{
  return &newChild(n);
}

void Node::remove_child(Node &)
{
  std::cerr<<"Warning: everything is deleted - maybe only one should be! (in xml.cpp Node::remove_child(Node &n)"<<std::endl;
  clear();
}

Node::iterator Node::begin()
{
  return mNodes.begin();
}
Node::const_iterator Node::begin() const
  {
    return mNodes.begin();
  }
Node::iterator Node::end()
{
  return mNodes.end();
}
Node::const_iterator Node::end() const
  {
    return mNodes.end();
  }

void Node::setContent(const std::string &s)
{
  mContent=s;
}

void Node::set(const std::string &pName,const std::string &pValue)
  {
    mParams[pName]=pValue;
  }
std::string Node::get(const std::string &pName) const
    {
      std::string v;
      std::map<std::string,std::string>::const_iterator i=mParams.find(pName);
      if(i!=mParams.end())
        return i->second;
      return "";//mParams[pName];
    }

void Node::clear()
{

  mNodes.clear();
  mParams.clear();
  mName="";
}

std::string Node::escape(const std::string &s) const
  {
    return replace(replace(s,"\\","\\\\"),"\"","\\\"");
  }
std::string Node::unescape(const std::string &s) const
  {
    return s;
    std::string n;
    size_t i;
    bool escape=false;
    for(i=0;i<s.length();i++)
      {
        if(s[i]=='\\')
          {
            if(escape)
              {
                escape=false;
                n+="\\";
              }
            else
              escape=true;
          }
        else if(s[i]=='\"')
          {
            if(escape)
              {
                n+="\"";
                escape=false;
              }
            else
              throw ParserException("Error in unescaping");
          }
        else
          {
            escape=false;
            n+=s[i];
          }
      }
    return n;
  }

void Node::getStart(std::ostringstream &s,bool complete) const
  {
    s<<"<"<<mName;
    if(mParams.size()>0)
      {
        std::map<std::string,std::string>::const_iterator i=mParams.begin();
        for(;i!=mParams.end();i++)
          s<<" "<<i->first<<"=\""<<escape(i->second)<<"\"";
      }
    if(complete)
      s<<"/";
    s<<">";
  }
void Node::getEnd(std::ostringstream &s) const
  {
    s<<"</"<<mName<<">";
  }

std::string Node::getContent() const
  {
    return mContent;
  }

void Node::indent(std::ostringstream &s,int depth) const
  {
    for(;depth>0;depth--)
      s<<" ";
  }

void Node::getContent(std::ostringstream &s,int depth) const
  {
    std::list<Node>::const_iterator i=mNodes.begin();
    for(;i!=mNodes.end();i++)
      {
        if(depth>0)
          {
            // width indenting
            indent(s,depth);
	    if(i->mNodes.size()==0 && i->mContent.length()==0)
	      i->getStart(s,true);
	    else
	      {
		i->getStart(s);
		s<<endl;
		i->getContent(s,depth+2);
		indent(s,depth);
		i->getEnd(s);
	      }
	    s<<endl;
	      
          }
        else
          {
            // without indenting
            i->getStart(s);
            i->getContent(s,0);
            i->getEnd(s);
          }
      }
    s<<mContent;
  }

std::string Node::toString(bool indent) const
  {
    std::ostringstream os;
    if(indent)
      {
        getStart(os);
        os<<std::endl;
        getContent(os,2);
        getEnd(os);
        os<<std::endl;
      }
    else
      {
        getStart(os);
        getContent(os,2);
        getEnd(os);
      }
    return os.str();
  }

void Node::parseChar(ParserInfo &info,char c)
{
  if(info.next()!=c)
    throw ParserException("Wrong char");
  info.inc();
}

std::string Node::parseString(ParserInfo &info,char delimit)
{
  bool weiter=true;
  bool escape=false;
  std::ostringstream n;

  while(weiter)
    {
      char c=info.next();
      if(c=='\\')
        {
          if(escape)
            {
              n<<"\\";
              escape=false;
            }
          else
            escape=true;
        }
      else if(c==delimit)
        {
          if(escape)
            {
              n<<delimit;
              escape=false;
            }
          else
            return n.str();
        }
      else if(c=='n' && escape)
	n<<'\n',escape=false;
      else
        n<<c;
      info.inc();
    }
  // never reached
  return "";
}

void Node::parseArguments(ParserInfo &info)
{
  // argument always starts with " "
  while(info.next()==' ')
    {
      while(info.next()==' ')
	parseChar(info,' ');
      std::string n,v;
      n=parseName(info);
      parseChar(info,'=');
      if(info.next()=='"')
	{
	  parseChar(info,'"');
	  v=parseString(info,'"');
	  parseChar(info,'"');
	}
      else
	{
	  parseChar(info,'\'');
	  v=parseString(info,'\'');
	  parseChar(info,'\'');
	}
      mParams[n]=unescape(v);
    }
}

std::string Node::parseName(ParserInfo &info)
{
  std::ostringstream os;
  char i=info.next();
  while(
    (i>='a' && i<='z') ||
    (i>='A' && i<='Z') ||
    (i>='0' && i<='9') ||
    (i=='ä' || i=='ö' || i=='ü') ||
    (i=='Ä' || i=='Ö' || i=='Ü') ||
    (i=='ß') ||
    (i=='_') ||
    (i=='-')
  )
    {
      os<<i;
      info.inc();

      i=info.next();
    }
  return os.str();
}

void Node::parseContents(ParserInfo &info)
{
  std::string la=info.getNext2();
  bool contentChanged=true;

  while(la.length() || contentChanged==true)
    {
      if(la[1]=='/')
        break;
      if(la[0]!='<')
        {
          mContent+=la[0];
          info.inc(); // inc only by one
          la=info.getNext2();
          contentChanged=true;
          //throw ParserException("ERROR in parseContents");
        }
      else
        {
          mNodes.push_back(Node());
          mNodes.back().parse(info);
          la=info.getNext2();
          contentChanged=false;
        }
    }
}

void Node::parse(ParserInfo &info)
{
  parseChar(info,'<');
  // check for comment
  if(info.getNext3()=="!--")
    {
      // FIXME: should be included somehow somewhen...
      info.inc();
      info.inc();
      info.inc();

      while(info.getNext3()!="-->")
	info.inc();
      info.inc();
      info.inc();
      info.inc();
    }
  else
    {
      
      mName=parseName(info);
      parseArguments(info);
      if(info.next()=='/')
	{
	  parseChar(info,'/');
	  parseChar(info,'>');
	}
      else
	{
	  parseChar(info,'>');
	  parseContents(info);
	  parseChar(info,'<');
	  parseChar(info,'/');
	  if(mName!=parseName(info))
	    throw ParserException("Wrong close-tag "+info.getInfo()+" name:"+mName);
	  parseChar(info,'>');
	}
    }
}

/****************************************************************
 * ParserException
 ****************************************************************/



Document::Document()
{}
Document::Document(std::string pFile)
{
  mRoot.clear();
  parseFile(pFile);
}

bool Document::parseFile(std::string file)
{
  std::string s;
  s=loadFile(file);

  parseMemory(s);
  return true;
}

Node &Document::root()
{
  return mRoot;
}

Node *Document::get_root_node()
{
  return &root();
}

std::string Document::toString() const
  {
    return mRoot.toString();
  }

void Document::parse_memory(const std::string &s)
{
  parseMemory(s);
}

void Document::parseHeader(ParserInfo &p)
{
  CTRACE;
  bool found=true;
  while(found)
    {
      found=false;
      if(p.getNext2()=="<?")
	{
	  while(p.getNext2()!="?>")
	    p.inc();
	  p.inc();
	  p.inc();
	  found=true;
	}
      else if(p.next()==' ' || p.next()=='\n')
	p.inc(),found=true;
    }
}

void Document::parseMemory(const std::string &s)
{
  ParserInfo p;
  p.s=s;
  p.p=0;

  parseHeader(p);
  mRoot.parse(p);
}

Document *Document::get_document()
{
  return this;
}



/****************************************************************
 * Compability functions
 ****************************************************************/


void setAttribute(Node *n,std::string name,std::string d)
{
  n->set
  (name,d);
}

std::string getAttribute(const Node &n,std::string name,std::string d)
{
  std::string v=n.get(name);
  if(v=="")
    v=d;
  return v;
}

std::string getAttribute(const Node *n,std::string name,std::string d)
{
  std::string v=n->get
                (name);
  if(v=="")
    v=d;
  return v;
}

std::string getAttributeF(const Node &n,std::string name,std::string d)
{
  std::string v=n.get(name);
  if(v=="")
    throw ParserException(d);
  return v;
}

std::string getAttributeF(const Node *n,std::string name,std::string d)
{
  std::string v=n->get
                (name);
  if(v=="")
    throw ParserException(d);
  return v;
}

int getAttribute(const Node &n,std::string name,int i)
{
  if(n.get(name)=="")
    return i;
  else
    return atoi(n.get(name).c_str());
}
int getAttribute(const Node *n,std::string name,int i)
{
  if(n->get
      (name)=="")
    return i;
  else
    return atoi(n->get
                (name).c_str());
}

Node *getRootNode(Document &doc)
{
  return &doc.root();
}

std::string toString(const Document &doc)
{
  return doc.toString();
}

Node *createRootNode(Document &doc,std::string n)
{
  doc.root().setName(n);
  return &doc.root();
}

