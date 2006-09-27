#ifndef AG_PLUGIN_H
#define AG_PLUGIN_H

#include <string>
#include <ag_rubyobj.h>

class AGEXPORT AGPlugin:public AGRubyObject
{
 public:
  virtual ~AGPlugin();

 private:
  void *myptr;
  friend AGPlugin *loadPlugin(const std::string &pFile);
};

AGPlugin *loadPlugin(const std::string &pFile);



#ifdef WIN32
#define AGPLEXPORT __declspec(dllexport)
#define AGPLIMPORT __declspec(dllimport)
#else
#define AGPLEXPORT
#define AGPLIMPORT
#endif


#define DECLARE_PLUGIN(name) extern "C" {AGPlugin * AGPLIMPORT create(); }
#define DEFINE_PLUGIN(name) extern "C" {AGPlugin * AGPLEXPORT create(){return new  name();} }

#endif
