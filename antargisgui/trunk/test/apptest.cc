#include "ag_application.h"
#include "ag_main.h"
#include "ag_button.h"


class MyApp:public AGApplication
{
public:
  bool eventIdle()
  {
    //    tryQuit();
    return true;
  }
};

int main()
{
  AGMain main;
  MyApp app;

  AGButton b(0,AGRect(0,0,50,20),"hallo");
  AGButton b1(&b,AGRect(0,20,50,20),"hallo");
  AGButton b2(&b,AGRect(0,40,50,20),"hallo");
  AGButton b3(&b,AGRect(0,60,50,20),"hallo");
  AGButton b4(&b,AGRect(0,80,50,20),"hallo");

  app.setMainWidget(&b);

  app.run();
}
