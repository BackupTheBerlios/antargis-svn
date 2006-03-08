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
#ifndef __WIN32__
#include "ag_regex.h"
#endif
#include "ag_debug.h"

/**
   Node
   @todo finish documentation
*/
Node::Node()
{
}
Node::Node(std::string name):mName(name)
{
}

Node::Node(const Node &n):mName(n.mName),mAttrs(n.mAttrs),mContent(n.mContent)
{
  // copy nodes
  NodeVector::const_iterator i=n.mNodes.begin();
  for(;i!=n.mNodes.end();i++)
    mNodes.push_back(new Node(**i));
}


/// somehow needed for swig-wrapping
Node::Node(const Node *n):mName(n->mName),mAttrs(n->mAttrs),mContent(n->mContent)
{
  // copy nodes
  NodeVector::const_iterator i=n->mNodes.begin();
  for(;i!=n->mNodes.end();i++)
    mNodes.push_back(new Node(**i));
}

Node::~Node()
{
  NodeVector::iterator i=mNodes.begin();
  for(;i!=mNodes.end();i++)
    delete *i;
  mNodes.clear();
}

void Node::setAttributes(const Attributes &pAttrs)
{
  mAttrs=pAttrs;
}


void Node::setName(std::string pName)
{
  mName=pName;
}

Node::NodeVector Node::getChildren() const
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

Node::NodeVector Node::getChildren(std::string pName) const
  {
    NodeVector l;
    NodeVector::const_iterator i=mNodes.begin();
    for(;i!=mNodes.end();i++)
      {
        if((*i)->getName()==pName)
          l.push_back(*i);
      }
    return l;
  }

std::string Node::getName() const
  {
    return mName;
  }

Node &Node::addChild(const std::string &pName)
{
  Node *node=new Node(pName);
  mNodes.push_back(node);
  return *mNodes.back();
}

void Node::removeChild(Node &)
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
    mAttrs[pName]=pValue;
  }
std::string Node::get(const std::string &pName) const
    {
      std::string v;
      std::map<std::string,std::string>::const_iterator i=mAttrs.find(pName);
      if(i!=mAttrs.end())
        return i->second;
      return "";//mAttrs[pName];
    }

void Node::clear()
{

  mNodes.clear();
  mAttrs.clear();
  mName="";
}

std::string Node::escape(const std::string &s)
  {
    return replace(replace(s,"\\","\\\\"),"\"","\\\"");
  }
std::string Node::unescape(const std::string &s)
  {
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
              throw std::string("Error in unescaping");
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
    if(mAttrs.size()>0)
      {
        std::map<std::string,std::string>::const_iterator i=mAttrs.begin();
        for(;i!=mAttrs.end();i++)
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

size_t Node::size() const
{
  return mNodes.size();
}

std::string Node::getContent() const
  {
    if(mContent.length()==0)
      {
	if(size()==1)
	  if((*mNodes.begin())->mContent.length()>0)
	    return (*mNodes.begin())->mContent;
      }
    return mContent;
  }

void Node::indent(std::ostringstream &s,int depth) const
  {
    for(;depth>0;depth--)
      s<<" ";
  }

bool Node::isTextNode() const
{
  return mName.length()==0 && mAttrs.size()==0;
}
std::string Node::getText() const
{
  return mContent;
}

bool Node::hasTextNode() const
{
  if(isTextNode())
    return true;
  
  NodeVector::const_iterator i=mNodes.begin();
  for(;i!=mNodes.end();i++)
    {
      Node *n=*i;
      if(n->hasTextNode())
	return true;
    }
  return false;
}



void Node::getContent(std::ostringstream &s,int depth) const
  {
    NodeVector::const_iterator i=mNodes.begin();
    for(;i!=mNodes.end();i++)
      {
	Node *n=*i;
	if(n->isTextNode())
	  s<<n->getText();
	else
	  {
	    if(depth>0)
	      {
		// width indenting
		indent(s,depth);
		if(n->mNodes.size()==0 && n->mContent.length()==0)
		  n->getStart(s,true);
		else
		  {
		    n->getStart(s);
		    s<<std::endl;
		    n->getContent(s,depth+2);
		    indent(s,depth);
		    n->getEnd(s);
		  }
		s<<std::endl;
		
	      }
	    else
	      {
		// without indenting
		n->getStart(s);
		n->getContent(s,0);
		n->getEnd(s);
	      }
	  }
      }
    s<<mContent;
  }

std::string Node::toString(bool indent) const
  {
    std::ostringstream os;
    cdebug(mName.length());
    if(mName.length()==0)
      return mContent;

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
        getContent(os,0);
        getEnd(os);
      }
    return os.str();
  }

////////////////////////////////////////////////////////////////////////
// Document
////////////////////////////////////////////////////////////////////////


Document::Document()
{
  mRoot=new Node;
}
Document::Document(std::string pFile)
{
  mRoot=new Node;
  mRoot->clear();
  parseFile(pFile);
}

Document::~Document()
{
  delete mRoot;
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
  return *mRoot;
}

/*
Node *Document::getRootNode()
{
  return mRoot;
  }*/

std::string Document::toString() const
  {
    if(mRoot->hasTextNode())
      return mRoot->toString(false);
    else
      return mRoot->toString();
  }
/*
void Document::parseMemory(const std::string &s)
{
  parseMemory(s);
}
*/

void Document::parseMemory(const std::string &s)
{
  DomParser p;
  p.parse(s,this);
}

/*Document *Document::get_document()
{
  return this;
  }*/




/******************************************************************
 * Parser
 ******************************************************************/

Parser::Data::Data()
{
  pos=0;
  line=1;
}


bool Parser::Data::first(const std::string &p) const
{
  if(s.length()>=p.length()+pos)
    {
      return (s.substr(pos,p.length())==p);
    }
  return false;
}

std::string Parser::Data::getFirst(size_t i) const
{
  std::ostringstream os;
  for(size_t k=pos;k<i+pos;k++)
    {
      if(k>=s.length())
	break;
      os<<s.substr(k,1);
    }
  return os.str();
}

void Parser::Data::eat(size_t i)
{
  if(pos+i>s.length())
    throw int();
  for(size_t j=0;j<i;j++)
    if(s[pos+j]=='\n')
      line++;

  pos+=i;
}

void Parser::Data::push()
{
  stack.push_back(pos);
  linestack.push_back(line);
}

void Parser::Data::pop()
{
  pos=stack.back();
  stack.pop_back();

  line=linestack.back();
  linestack.pop_back();
}

void Parser::Data::discard()
{
  stack.pop_back();
  linestack.pop_back();
}

void Parser::Data::eatBlanks()
{
  std::string f=getFirst(1);

  while(f==" " || f=="\t" || f=="\n")
    {
      eat(1);
      f=getFirst(1);
    }

}

std::string Parser::Data::getTil(const std::string &p) const
{
  size_t i=s.find(p,pos);
  if(i!=s.npos)
    return s.substr(pos,i-pos);
  return "";
}

Parser::~Parser()
{
}

size_t Parser::getLine() const
{
  return data.line;
}

void Parser::parse(const std::string &pData)
{
  data.s=pData;

  parseHeader();
  while(!eod())
    {
      //      cdebug(data.pos<<":"<<data.getFirst(5));
      if(!parseComment())
	if(!parseNodeEnd())
	  {
	    NodeStartInfo i=parseNodeStart();
	    if(!i.startTag)
	      if(!parseText())
		{
		  cdebug("ERROR AT line:"<<getLine()<<":"<<data.getFirst(20));
		  break;
		}
	  }
    }
}

bool Parser::parseText()
{
  std::ostringstream os;
  while(data.getFirst(1)!="<" && data.getFirst(1).length())
    {
      os<<data.getFirst(1);
      data.eat(1);
    }
  if(os.str().length())
    {
      text(os.str());
      return true;
    }
  return false;
}

bool Parser::parseComment()
{
  if(data.first("<!--"))
    {
      data.eat(4);
      std::string s=data.getTil("-->");
      comment(s);
      data.eat(s.length()+3);
      return true;
    }
  return false;
}

std::string Parser::parseName()
{
  std::ostringstream os;
  std::string first=data.getFirst(1);
  while(first!=" " && first!="\t" && first!=">" && first!="/")
    {
      os<<first;
      data.eat(1);
      first=data.getFirst(1);
    }
  return os.str();
}

Parser::NodeStartInfo Parser::parseNodeStart()
{
  NodeStartInfo info;
  info.startTag=false;
  if(!data.first("<"))
    return info;

  info.startTag=true;
  data.eat(1);
  std::string name=parseName();
  //  cdebug("name:"<<name);
  //  data.eat(name.length());
  
  Node::Attributes attrs=parseAttributes();

  //  cdebug("pos:"<<data.pos<<":"<<data.getFirst(5));
  data.eatBlanks();
  if(data.first("/"))
    {
      //      cdebug("simple");
      info.simple=true;
      data.eat(1);
    }
  else
    info.simple=false;
  assert(data.first(">"));
  data.eat(1);
  if(info.simple)
    simpleTag(name,attrs);
  else
    startTag(name,attrs);
  return info;
}


bool Parser::parseNodeEnd()
{
  if(!data.first("</"))
    return false;
  
  data.eat(2);
  std::string name=data.getTil(">");
  data.eat(name.length()+1);
  endTag(name);
  return true;
}

bool Parser::parseHeader()
{
  //  cdebug("header:"<<data.first("<?"));
  //  cdebug("header:"<<data.getFirst(2));
  if(!data.first("<?"))
    return false;
  data.push();
  data.eat(2);
  std::string c=data.getTil("?>");
  //  cdebug("c:"<<c);
  if(c.length()==0)
    {
      data.pop();
      return false;
    }
  data.eat(c.length());
  if(data.first("?>"))
    {
      header(c);
      data.discard();
      data.eat(2);
      return true;
    }
  data.pop();
  return false;
}

std::string Parser::parseString()
{
  std::string start=data.getFirst(1);
  std::string first;
  std::ostringstream os;
  data.eat(1);

  bool escape, sescape;
  escape=sescape=false;

  do
    {
      first=data.getFirst(1);
      data.eat(1);
      if(escape)
	{
	  escape=false;
	  os<<"\\"<<first;
	}
      else if(first=="\\")
	{
	  escape=true;
	}
      else
	{
	  if(first==start)
	    break;
	  else
	    os<<first;
	}
      
    }while(true);
  
  return os.str();
}

Node::Attributes Parser::parseAttributes()
{
  Node::Attributes attrs;
  std::string name,value;
  //  cdebug("first:"<<data.getFirst(1));

  while(!(data.first(">") || data.first("/")))
    {
      data.eatBlanks();
      name=data.getTil("=");
      data.eat(name.length()+1);

      value=parseString();

      //      cdebug("attr:"<<name<<" = "<<value);
      attrs[name]=value;
    }
  return attrs;
}

bool Parser::eod()
{
  return data.pos>=data.s.length();
}

void Parser::simpleTag(const std::string &pName,const Node::Attributes &pAttributes)
{
  cdebug("simple:"<<pName);
}

void Parser::startTag(const std::string &pName,const Node::Attributes &pAttributes)
{
  cdebug("start:"<<pName);
}
void Parser::endTag(const std::string &pName)
{
  cdebug("end:"<<pName);
}
void Parser::text(const std::string &pText)
{
  cdebug("text:"<<pText);

}
void Parser::comment(const std::string &pText)
{
  cdebug("comment:"<<pText);
}



void Parser::header(const std::string &pText)
{
  cdebug("header:"<<pText);
}


/////////////////////////////////////////////////////////////////////////////////////
// DomParser
/////////////////////////////////////////////////////////////////////////////////////

void DomParser::simpleTag(const std::string &pName,const Node::Attributes &pAttributes)
{
  if(nodes.size()==0)
    {
      Node &n=doc->root();

      n.setName(pName);
      n.setAttributes(pAttributes);
    }
  else
    {
      Node *p=nodes.back();
      Node &n=p->addChild(pName);
      n.setAttributes(pAttributes);
    }
}
void DomParser::startTag(const std::string &pName,const Node::Attributes &pAttributes)
{
  if(nodes.size()==0)
    {
      Node &n=doc->root();

      n.setName(pName);
      n.setAttributes(pAttributes);
      nodes.push_back(&n);
    }
  else
    {
      Node *p=nodes.back();
      Node &n=p->addChild(pName);
      n.setAttributes(pAttributes);
      nodes.push_back(&n);
    }
}
void DomParser::endTag(const std::string &pName)
{
  if(nodes.size()==0)
    {
      std::cerr<<"ERROR:close tag for '"<<pName<<"' and no start tag at all for this! line:"<<getLine()<<std::endl;
    }
  else
    {
      if(nodes.back()->getName()!=pName)
	{
	  std::cerr<<"ERROR: close tag for '"<<pName<<"' found, but '"<<nodes.back()->getName()<<"' expected! line:"<<getLine()<<std::endl;
	}
      else
	{
	  nodes.pop_back();
	}
    }
}
void DomParser::text(const std::string &pText)
{
  if(nodes.size()>0) // ignore text otherwise
    {
      nodes.back()->addChild("").setContent(pText);
    }
}
void DomParser::comment(const std::string &pText)
{
  nodes.back()->addChild("").setContent(std::string("<!--")+pText+std::string("-->"));
}
void DomParser::header(const std::string &pText)
{
  // do nothing
}


Document *DomParser::parse(const std::string &pData,Document *d)
{
  doc=d;
  nodes.clear();
  Parser::parse(pData);

  nodes.clear();
  return doc;
}

Document *DomParser::parse(const std::string &pData)
{
  return parse(pData,new Document);
}
