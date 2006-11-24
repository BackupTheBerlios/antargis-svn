#include "ag_destructor.h"


AGDestructionHandler::~AGDestructionHandler()
{
  for(std::set<AGDestructionSender*>::iterator i=mSenders.begin();i!=mSenders.end();i++)
    (*i)->resetHandler();

}
void AGDestructionHandler::destroyed(AGDestructionSender *s, const std::string &pName)
{
}

void AGDestructionHandler::addSender(AGDestructionSender *s)
{
  mSenders.insert(s);
}
void AGDestructionHandler::subSender(AGDestructionSender *s)
{
  if(mSenders.find(s)!=mSenders.end())
    {
      mSenders.erase(s);
      destroyed(s,s->mName);
    }
}


AGDestructionSender::AGDestructionSender(AGDestructionHandler *pHandler,const std::string &pName):mName(pName),mHandler(pHandler)
{
  if(mHandler)
    mHandler->addSender(this);
}
AGDestructionSender::~AGDestructionSender()
{
  if(mHandler)
    mHandler->subSender(this);
}


void AGDestructionSender::resetHandler()
{
  mHandler=0;
}
