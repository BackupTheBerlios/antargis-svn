#ifndef AG_MESSAGE_H_
#define AG_MESSAGE_H_

#include <ag_string.h>
#include <map>

class AGMessageSender
{
  
};

class AGMessageReceiver
{
  
};

class AGMessageTransceiver
{
  
};

class AGMessageSource
{
  AGMessageSender *mSender;
  AGString mName;
public:
};

class AGMessage
{
  AGMessageSource *mSource;
public:
  AGMessage(const AGMessageSource *pSource);
};

class AGMessageQueue
{
  std::map<AGMessageSource,std::list<AGMessageReceiver*> > connects;
public:
  void connect(const AGMessageSource &pSource,const AGMessageReceiver *pReceiver);
  void distconnect(const AGMessageSource &pSource,const AGMessageReceiver *pReceiver);
  
  void send(const AGMessage &pMessage);
};

#endif /*AG_MESSAGE_H_*/
