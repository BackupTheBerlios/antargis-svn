/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_xml.h
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

#ifndef __MY_SIMPLE_XML
#define __MY_SIMPLE_XML

#include <ag_tools.h>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include <ruby.h>

//#include "mystring.h"
#include "ag_debug.h"

class ParserException
  {
    std::string s;
  public:
    ParserException(std::string u);
    std::string what();
  };

struct ParserInfo
  {
    std::string s;
    size_t p;

    char next();
    std::string getNext2();
    std::string getNext3();
    void inc();
    bool end();

    std::string getInfo();
  };

class Node
  {
    std::string mName;
    std::vector<Node*> mNodes;
    std::map<std::string,std::string> mParams;
    std::string mContent;

  public:

    typedef std::vector<Node*> NodeVector;

    typedef NodeVector::iterator iterator;
    typedef NodeVector::const_iterator const_iterator;

    Node();
    Node(std::string name);
    Node(const Node &n);
    ~Node();

    void setName(std::string pName);

    NodeVector get_children() const;

#ifndef SWIG
    const Node &(operator*)() const;
    const Node *(operator->)() const;
#endif
    NodeVector get_children(std::string pName) const;
    
    std::string getName() const;
    std::string get_name() const; // Compability

    Node &newChild(std::string pName);

    Node *add_child(std::string n);

    void remove_child(Node &n);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    void setContent(const std::string &s);
    std::string getContent() const;

    void set(const std::string &pName,const std::string &pValue);
    std::string get(const std::string &pName) const;
    void clear();

    std::string escape(const std::string &s) const;
    std::string unescape(const std::string &s) const;

    // dumping functions
    void getStart(std::ostringstream &s,bool complete=false) const;
    void getEnd(std::ostringstream &s) const;
      
    void indent(std::ostringstream &s,int depth) const;
    void getContent(std::ostringstream &s,int depth) const;
    std::string toString(bool indent=true) const;
    void parseChar(ParserInfo &info,char c);
    
    // parse functions
    std::string parseString(ParserInfo &info,char delimit);
    void parseArguments(ParserInfo &info);
    std::string parseName(ParserInfo &info);
    void parseContents(ParserInfo &info);
    void parse(ParserInfo &info);

  };

class Document
  {
    Node *mRoot;
  public:
    Document();
    Document(std::string pFile);
    ~Document();

    bool parseFile(std::string file);

    Node &root();

    Node *get_root_node();

    std::string toString() const;

    void parse_memory(const std::string &s);

    void parseMemory(const std::string &s);
    
    Document *get_document();


  private:
    void parseHeader(ParserInfo &p);
  };


// compability with libxml++ and my extensions

namespace myxmlpp
  {
  typedef ::Node Node;
  typedef ::Document DomParser;
};
namespace xmlpp
  {
  typedef ::Node Node;
  typedef ::Document DomParser;
  typedef ::Document Document;//DomParser;
};

/****************************************************************
 * Compability functions
 ****************************************************************/
#define checkInclude(x,y)

void setAttribute(Node *n,std::string name,std::string d);
std::string getAttribute(const Node &n,std::string name,std::string d);
std::string getAttribute(const Node *n,std::string name,std::string d);
std::string getAttributeF(const Node &n,std::string name,std::string d);
std::string getAttributeF(const Node *n,std::string name,std::string d);
int getAttribute(const Node &n,std::string name,int i);
int getAttribute(const Node *n,std::string name,int i);
Node *getRootNode(Document &doc);
std::string toString(const Document &doc);
Node *createRootNode(Document &doc,std::string n);

#endif
