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

#include "ag_debug.h"

class Node
  {
  public:
    typedef std::map<std::string,std::string> Attributes;

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

    void setAttributes(const Attributes &pAttrs);

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
    
    bool isTextNode() const;
    std::string getText() const;

    bool hasTextNode() const;

  private:
    std::string mName;
    std::vector<Node*> mNodes;
    Attributes mAttrs;
    std::string mContent;
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


  };

class Parser
{
  struct Data
  {
    std::list<size_t> stack;
    std::list<size_t> linestack;
    size_t pos;
    std::string s;
    size_t line;

    Data();

    bool first(const std::string &p) const;
    std::string getFirst(size_t i) const;
    void eat(size_t i);
    void push();
    void pop();
    void discard();
    std::string getTil(const std::string &p) const;
    void eatBlanks();
  };

  struct NodeStartInfo
  {
    bool startTag;
    bool simple; // == <.../>
    std::string name;
  };

  Data data;

  NodeStartInfo parseNodeStart();
  bool parseNodeEnd();
  void parseNodeContent();
  bool parseHeader();
  std::string parseString();
  Node::Attributes parseAttributes();
  bool parseComment();
  bool parseText();
  void parseNode();
  std::string parseName();

  bool eod();

 public:
  virtual ~Parser();

  void parse(const std::string &pData);
  size_t getLine() const;

  virtual void simpleTag(const std::string &pName,const Node::Attributes &pAttributes);
  virtual void startTag(const std::string &pName,const Node::Attributes &pAttributes);
  virtual void endTag(const std::string &pName);
  virtual void text(const std::string &pText);
  virtual void comment(const std::string &pText);
  virtual void header(const std::string &pText);
};

class DomParser:public Parser
{
  Document *doc;
  std::list<Node*> nodes;
 public:
  virtual void simpleTag(const std::string &pName,const Node::Attributes &pAttributes);
  virtual void startTag(const std::string &pName,const Node::Attributes &pAttributes);
  virtual void endTag(const std::string &pName);
  virtual void text(const std::string &pText);
  virtual void comment(const std::string &pText);
  virtual void header(const std::string &pText);

  Document *parse(const std::string &pData);

  Document *parse(const std::string &pData,Document *d);
};
  
namespace xmlpp
{
  typedef ::Node Node;
  typedef ::Document Document;
};

#endif
