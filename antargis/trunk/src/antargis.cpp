#include "fs.h"

#include "ag_application.h"
#include "ag_messageobject.h"
#include "ag_main.h"
#include "ag_button.h"
#include "ag_surface.h"
#include "ag_debug.h"
#include "ag_theme.h"

#include "myapp.h"
#include "voxel.h"

#include "map.h"
#include "view.h"

#include <exception>
#include <iostream>

void setTheme()
{
  AGTheme *t=getTheme();
  AGGScreen &s=getScreen();
  t->setSurface("buttonbg.normal",s.loadSurface("data/rnew_button_bg0.png"));
  t->setSurface("buttonbg.lighted",s.loadSurface("data/rnew_button_bg1.png"));
  t->setSurface("buttonbg.pressed",s.loadSurface("data/rnew_button_bg2.png"));
  t->setSurface("buttonbg.checked",s.loadSurface("data/rnew_button_bg3.png"));
  t->setInt("borderWidth.button",0);
  t->setInt("buttonLowerOnClick",1);
}


class AntargisApp:public MyApp
{
  AntargisMap map;
  int w,h;
  bool paused;
  EditIsoView *mainView;
  
  public:
  AntargisApp(int W,int H):map(64,64),w(W),h(H)
  {
    paused=false;
  }
  void run()
  {
  
  map.insertEntity(new AntTree(Pos2D(100,100),0));
  map.insertEntity(new AntTree(Pos2D(200,102),0));
  map.insertEntity(new AntTree(Pos2D(300,402),0));
  
  map.insertEntity(new AntTree(Pos2D(600,402),1));
  map.insertEntity(new AntTree(Pos2D(400,902),2));
  
  AntHero *hero;
  
  map.insertEntity(hero=new AntHero(Pos2D(700,702),0,"David"));
  
  map.insertEntity(new AntMan(Pos2D(600,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(800,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(500,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(900,600),0,hero));
  
  map.insertEntity(new AntMan(Pos2D(600,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(800,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(500,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(900,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(600,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(800,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(500,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(900,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(600,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(800,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(500,600),0,hero));
  map.insertEntity(new AntMan(Pos2D(900,600),0,hero));
  
  map.insertEntity(hero=new AntHero(Pos2D(200,702),1,"Brutus"));
  
  //map.insertEntity(new AntMan(Pos2D(200,600),1,hero));
  //map.insertEntity(new AntMan(Pos2D(230,600),1,hero));
  
  AntPlayer player(1);
  player.insertHero(hero);
  
#ifdef EDITING
  mainView=new EditIsoView(0,AGRect(0,0,w,h),Pos3D(0,0,0),&map);
  map.pause();
#else
  mainView=new CompleteIsoView(0,AGRect(0,0,w,h),Pos3D(0,0,0),&map);
#endif  
  AGButton *b;
  mainView->addChild(b=new AGButton(mainView,AGRect(0,0,50,50),"test Window"));
  b->setSurface(getScreen().loadSurface("data/pointer1.png"),false);
  
  // Edit
  mainView->addChild(b=new AGButton(mainView,AGRect(0,50,50,50),"test Window"));
  b->setSurface(getScreen().loadSurface("data/edit.png"),false);
  b->sigClick.connect(slot(this,&AntargisApp::toggleEdit));
  
  mainView->addChild(b=new AGButton(mainView,AGRect(0,200,50,50),"test Widnow"));
  b->setSurface(getScreen().loadSurface("graphics/white_pin.png"),false);
  b->sigClick.connect(slot(this,&AntargisApp::pinClick));
  
  mainView->addChild(b=new AGButton(mainView,AGRect(0,250,50,50),"test Widnow"));
  b->setSurface(getScreen().loadSurface("data/small_tree.png"),false);
  b->sigClick.connect(slot(this,&AntargisApp::pinClick));
  
  
  // SmainViewe
  mainView->addChild(b=new AGButton(mainView,AGRect(0,100,50,50),"test Window"));
  b->setSurface(getScreen().loadSurface("data/save.png"),false);
  b->sigClick.connect(slot(this,&AntargisApp::saveClick));
  
  // load
  mainView->addChild(b=new AGButton(mainView,AGRect(0,150,50,50),"test Window"));
  b->setSurface(getScreen().loadSurface("data/load.png"),false);
  b->sigClick.connect(slot(this,&AntargisApp::loadClick));

  // any
  mainView->addChild(b=new AGButton(mainView,AGRect(1023-50,0,50,50),""));
  b->setSurface(getScreen().loadSurface("data/door.png"),false);
  b->sigClick.connect(slot(this,&AntargisApp::quitClick));
  
  // Pause
  mainView->addChild(b=new AGButton(mainView,AGRect(1023-100,0,50,50),""));
  b->setSurface(getScreen().loadSurface("data/pause.png"),false);
  b->sigClick.connect(slot(this,&AntargisApp::pause));


  setMainWidget(mainView);
    
  
    MyApp::run();
  }
  
  bool toggleEdit(const char *,const AGEvent *)
  {
    mainView->toggleEdit();
    getMap()->pause();
    return true;
  }
  
  bool saveClick(const char *,const AGEvent *)
  {
    getMap()->saveMap("dummy.antlvl");
    return true;
  }
  bool pinClick(const char *,const AGEvent *)
  {
    mainView->toggleShowPoints();
    return true;
  }
  bool loadClick(const char *,const AGEvent *)
  {
    getMap()->loadMap("dummy.antlvl");
    mainView->update();
    return true;
  }
  bool quitClick(const char *,const AGEvent *)
  {
    tryQuit();
    return true;
  }
  bool pause(const char *,const AGEvent *)
  {
    CTRACE;
    paused=!paused;
    if(paused)
      getMap()->pause();
    else
      getMap()->unpause();
    return false;
  }
  
};

int main(int argc,char *argv[])
{
  try
  {
  assert(argc>=1);
  initFS(argv[0]);

  AGMain main;

  int w=1024;
  int h=768;

  main.changeRes(w,h,32,false,true);

  setTheme();
  
  AntargisApp app(w,h);
  

  app.run();
  }
  catch(std::exception &e)
  {
    std::cout<<e.what()<<std::endl;
  }
  return 0;
}



