#include "ag_application.h"
#include "ag_main.h"
#include "ag_button.h"
#include "ag_debug.h"
#include "SDL.h"
#include <iostream>
#include <exception>

class MyApp:public AGApplication
{
public:
  bool eventIdle()
  {
    //    tryQuit();
    return true;
  }
};

int main(int argc,char *argv[])
{
    TRACE;
 try
 {
  AGMain main;
  MyApp app;

  AGButton b(0,AGRect(0,0,50,20),"hallo");
  AGButton b1(&b,AGRect(0,20,50,20),"hallo");
  AGButton b2(&b,AGRect(0,40,50,20),"hallo");
  AGButton b3(&b,AGRect(0,60,50,20),"hallo");
  AGButton b4(&b,AGRect(0,80,50,20),"hallo");
{
                                             TRACE;
  app.setMainWidget(&b);
}
                                             {
                                                   TRACE;
 
  app.run();
}
}
catch(std::exception &x)
{
std::cerr<<x.what()<<std::endl;
}
}
