
#include <ag_collector.h>
#include <ag_debug.h>
#include <ag_main.h>

void AGCollector::insertGlobal(AGRubyObject *pObject)
{
  //  CTRACE;
  mGlobals.insert(pObject);
}

void AGCollector::removeGlobal(AGRubyObject *pObject)
{
  //  CTRACE;
  mGlobals.erase(pObject);
}

void AGCollector::mark()
{
  //  CTRACE;
  for(std::set<AGRubyObject*>::iterator i=mGlobals.begin();i!=mGlobals.end();i++)
    markObject(*i);
}

AGCollector *getCollector()
{
  if(getMain())
    return getMain()->getCollector();
  return 0;
}
