#include "fs.h"

#include "ag_application.h"
#include "ag_messageobject.h"
#include "ag_main.h"
#include "ag_button.h"
#include "ag_surface.h"
#include "ag_debug.h"
#include "ag_theme.h"

#include <ag_layout.h>
#include <ag_tools.h>


#include "minimap.h"
#include "myapp.h"

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
    MiniMap *miniMap;

  public:
    AntargisApp(int W,int H):map(128,128),w(W),h(H)
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
      map.insertEntity(new AntTree(Pos2D(600,902),3));
      map.insertEntity(new AntTree(Pos2D(900,902),4));
      map.insertEntity(new AntTree(Pos2D(700,902),5));

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

      AntPlayer player(1);
      player.insertHero(hero);

      int minimapsize=216;

      AGLayout *layout;
      setMainWidget(layout=new AGLayout(0,getFile("game_layout.xml")));

      layout->getChild("load")->sigClick.connect(slot(this,&AntargisApp::loadClick));
      layout->getChild("save")->sigClick.connect(slot(this,&AntargisApp::saveClick));
      layout->getChild("pins")->sigClick.connect(slot(this,&AntargisApp::pinClick));
      layout->getChild("edit")->sigClick.connect(slot(this,&AntargisApp::toggleEdit));
      layout->getChild("quit")->sigClick.connect(slot(this,&AntargisApp::quitClick));
      layout->getChild("pause")->sigClick.connect(slot(this,&AntargisApp::pause));

      layout->getChild("tree")->sigClick.connect(slot(this,&AntargisApp::pinClick)); // FIXME

      mainView=dynamic_cast<EditIsoView*>(layout->getChild("mainView"));

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
      mainView->completeUpdate();
      miniMap->update();
      return true;
    }
    bool mapEdited(const char *,const AGEvent *)
    {
      miniMap->update();
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
  catch(std::string &s)
    {
      std::cout<<s<<std::endl;
    }

  delete getVoxelID(); // FIXME: eliminate Singletons
  return 0;
}



