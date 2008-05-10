#ifndef AG_COLLECTOR_H
#define AG_COLLECTOR_H

#include <ag_base.h>
#include <ag_rubyobj.h>

#include <set>

class AGEXPORT AGCollector:public AGRubyObject
{
 public:
  void insertGlobal(AGRubyObject *pObject);
  void removeGlobal(AGRubyObject *pObject);

  virtual void mark();
 private:
  std::set<AGRubyObject*> mGlobals;
  
};

AGEXPORT AGCollector *getCollector();


#endif
