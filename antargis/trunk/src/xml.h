#ifndef __MY_SIMPLE_XML
#define __MY_SIMPLE_XML

#include <list>
#include <map>
#include <string>
#include <sstream>

#include "mystring.h"
#include "ag_debug.h"

class ParserException
  {
    std::string s;
  public:
    ParserException(std::string u):s(u)
    {
      CTRACE;
      cdebug("u:"<<u);
    }

    std::string what()
    {
      return s;
    }
  };

struct ParserInfo
  {
    std::string s;
    size_t p;

    char next()
    {
      if(s.length()>p)
        return s[p];
      else
        return 0;
    }
    std::string getNext2()
    {
      if(s.length()>p+1)
        return s.substr(p,2);
      return "";
    }
    void inc()
    {
      if(s.length()>p)
        p++;
    }
    bool end()
    {
      return(p>=s.length());
    }
  };

class Node
  {
    std::string mName;
    std::list<Node> mNodes;
    std::map<std::string,std::string> mParams;
    std::string mContent;

  public:

    typedef std::list<Node> NodeList;

    typedef std::list<Node>::iterator iterator;
    typedef std::list<Node>::const_iterator const_iterator;

    Node()
    {}
    Node(std::string name):mName(name)
    {}

    void setName(std::string pName)
    {
      mName=pName;
    }

    NodeList get_children() const
      {
        return mNodes;
      }

    const Node &(operator*)() const
      {
        return *this;
      }
    const Node *(operator->)() const
      {
        return this;
      }

    NodeList get_children(std::string pName) const
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

    std::string getName() const
      {
        return mName;
      }
    std::string get_name() const
      {
        return mName;
      }

    Node &newChild(std::string pName)
    {
      mNodes.push_back(Node(pName));
      return mNodes.back();
    }

    Node *add_child(std::string n)
    {
      return &newChild(n);
    }

    void remove_child(Node &n)
    {
      clear();
    }

    iterator begin()
    {
      return mNodes.begin();
    }
    const_iterator begin() const
      {
        return mNodes.begin();
      }
    iterator end()
    {
      return mNodes.end();
    }
    const_iterator end() const
      {
        return mNodes.end();
      }

    void setContent(const std::string &s)
    {
      mContent=s;
    }

    void set
      (std::string pName,std::string pValue)
      {
        mParams[pName]=pValue;
      }
    std::string get
      (std::string pName) const
        {
          std::string v;
          std::map<std::string,std::string>::const_iterator i=mParams.find(pName);
          if(i!=mParams.end())
            return i->second;
          return "";//mParams[pName];
        }

    void clear()
    {

      mNodes.clear();
      mParams.clear();
      mName="";
    }

    std::string escape(std::string s) const
      {
        return replace(replace(s,"\\","\\\\"),"\"","\\\"");
      }
    std::string unescape(std::string s) const
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

    void getStart(std::ostringstream &s) const
      {
        s<<"<"<<mName;
        if(mParams.size()>0)
          {
            std::map<std::string,std::string>::const_iterator i=mParams.begin();
            for(;i!=mParams.end();i++)
              s<<" "<<i->first<<"=\""<<escape(i->second)<<"\"";
          }
        s<<">";
      }
    void getEnd(std::ostringstream &s) const
      {
        s<<"</"<<mName<<">";
      }
      
      std::string getContent() const
      {
        return mContent;
      }

    void getContent(std::ostringstream &s) const
      {
        std::list<Node>::const_iterator i=mNodes.begin();
        for(;i!=mNodes.end();i++)
          {
            i->getStart(s);
            i->getContent(s);
            i->getEnd(s);
          }
        s<<mContent;
      }

    std::string toString() const
      {
        std::ostringstream os;
        getStart(os);
        getContent(os);
        getEnd(os);
        return os.str();
      }

    void parseChar(ParserInfo &info,char c)
    {
      if(info.next()!=c)
        throw ParserException("Wrong char");
      info.inc();
    }

    std::string parseString(ParserInfo &info)
    {
      bool weiter=true;
      bool escape=false;
      std::string n;

      while(weiter)
        {
          char c=info.next();
          if(c=='\\')
            {
              if(escape)
                {
                  n+="\\";
                  escape=false;
                }
              else
                escape=true;
            }
          else if(c=='\"')
            {
              if(escape)
                {
                  n+="\"";
                  escape=false;
                }
              else
                return n;
            }
          else
            n+=c;
          info.inc();
        }
      // never reached
      return "";
    }

    void parseArguments(ParserInfo &info)
    {
      // argument always starts with " "
      while(info.next()==' ')
        {
          parseChar(info,' ');
          std::string n,v;
          n=parseName(info);
          parseChar(info,'=');
          parseChar(info,'"');
          v=parseString(info);
          parseChar(info,'"');
          mParams[n]=unescape(v);
        }
    }

    std::string parseName(ParserInfo &info)
    {
      std::string n;
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
          n+=i;
          info.inc();

          i=info.next();
        }
      return n;
    }

    void parseContents(ParserInfo &info)
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

    void parse(ParserInfo &info)
    {
      parseChar(info,'<');
      mName=parseName(info);
      parseArguments(info);
      parseChar(info,'>');
      parseContents(info);
      parseChar(info,'<');
      parseChar(info,'/');
      if(mName!=parseName(info))
        throw ParserException("Wrong close-tag");
      parseChar(info,'>');
    }


  };

class Document
  {
    Node mRoot;
  public:
    Document()
    {}
    Document(std::string pFile)
    {
      mRoot.clear();
      parseFile(pFile);
    }

    void parseFile(std::string file)
    {
      std::string s;
      char buffer[1001];
      FILE *f=fopen(file.c_str(),"r");
      while(!feof(f))
        {
          size_t anzahl=fread(buffer,1,1000,f);
          if(anzahl==0)
            break;
          s+=std::string(buffer,anzahl);
        }
      fclose(f);

      parseMemory(s);
    }

    Node &root()
    {
      return mRoot;
    }

    Node *get_root_node()
    {
      return &root();
    }

    std::string toString() const
      {
        return mRoot.toString();
      }

    void parse_memory(const std::string &s)
    {
      parseMemory(s);
    }

    void parseMemory(const std::string &s)
    {
      ParserInfo p;
      p.s=s;
      p.p=0;
      mRoot.parse(p);
    }

    Document *get_document()
    {
      return this;
    }
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

#define checkInclude(x,y)

inline void setAttribute(Node *n,std::string name,std::string d)
{
  n->set
  (name,d);
}

inline std::string getAttribute(const Node &n,std::string name,std::string d)
{
  std::string v=n.get(name);
  if(v=="")
    v=d;
  return v;
}

inline std::string getAttribute(const Node *n,std::string name,std::string d)
{
  std::string v=n->get
                (name);
  if(v=="")
    v=d;
  return v;
}

inline std::string getAttributeF(const Node &n,std::string name,std::string d)
{
  std::string v=n.get(name);
  if(v=="")
    throw ParserException(d);
  return v;
}

inline std::string getAttributeF(const Node *n,std::string name,std::string d)
{
  std::string v=n->get
                (name);
  if(v=="")
    throw ParserException(d);
  return v;
}

inline int getAttribute(const Node &n,std::string name,int i)
{
  if(n.get(name)=="")
    return i;
  else
    return atoi(n.get(name).c_str());
}
inline int getAttribute(const Node *n,std::string name,int i)
{
  if(n->get
      (name)=="")
    return i;
  else
    return atoi(n->get
                (name).c_str());
}

inline Node *getRootNode(Document &doc)
{
  return &doc.root();
}

inline std::string toString(const Document &doc)
{
  return doc.toString();
}

inline Node *createRootNode(Document &doc,std::string n)
{
  doc.root().setName(n);
  return &doc.root();
}

#endif
