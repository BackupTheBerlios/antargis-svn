#ifndef MYAPP_H
#define MYAPP_H

#include "map.h"

class MyApp:public AGApplication
{
public:

  virtual bool eventFrame(float pTime)
  {
    getMap()->move(pTime);
    return true;
  }
  
  bool eventIdle()
  {
    SDL_Delay(20);
    //    tryQuit();
    return true;
  }
};

#endif
