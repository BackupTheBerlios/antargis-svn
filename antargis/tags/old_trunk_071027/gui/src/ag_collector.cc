
#include <ag_collector.h>

void AGCollector::insertGlobal(AGRubyObject *pObject)
{
  mGlobals.insert(pObject);
}

void AGCollector::removeGlobal(AGRubyObject *pObject)
{
  mGlobals.erase(pObject);
}

void AGCollector::mark()
{
  for(std::set<AGRubyObject*>::iterator i=mGlobals.begin();i!=mGlobals.end();i++)
    markObject(*i);
}
