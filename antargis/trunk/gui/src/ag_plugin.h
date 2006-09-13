#ifndef AG_PLUGIN_H
#define AG_PLUGIN_H

#include <string>
#include <ag_rubyobj.h>

class AGPlugin:public AGRubyObject
{
 public:
  virtual ~AGPlugin();

 private:
  void *myptr;
  friend AGPlugin *loadPlugin(const std::string &pFile);
};

AGPlugin *loadPlugin(const std::string &pFile);

#ifdef WIN32
#define AGEXPORT __declspec(dllexport)
#else
#define AGEXPORT
#endif

#define DECLARE_PLUGIN(name) extern "C" {AGPlugin * AGEXPORT create(); }
#define DEFINE_PLUGIN(name) extern "C" {AGPlugin * AGEXPORT create(){return new  name();} }

#endif
