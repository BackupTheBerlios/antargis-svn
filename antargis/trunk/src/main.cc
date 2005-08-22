#include "ag_application.h"
#include "ag_main.h"
#include <ag_button.h>
#include <fstream>

#include "tree.h"
#include "terrain.h"
#include "water.h"
#include "vertex_array.h"
#include "pbuffer.h"
#include "antapp.h"

#include "mesh.h"
std::ofstream out("log.txt");

void initRand()
{
  static bool inited=false;
  if(!inited)
    {
      inited=true;
      time_t zeit;
      time(&zeit);
      srand((int)zeit);
    }
}


class TestApp:public GLApp
{
public:
  TestApp(int w,int h):
    GLApp(w,h)
  {
    AntMap map(64,64);

    Document d;
    map.saveXML(d.root());
    cdebug(d.toString());
    map.loadXML(d.root());

    //    MeshData *workshop=new MeshData("data/werkstatt.ant",1.6);
    MeshData *farm=new MeshData("data/farm.ant",2.3);
    //    MeshData *tree=new MeshData("data/grass.ant",0.05); 
    MeshData *man=new MeshData("data/man.ant",0.8); 
    MeshData *stones=new MeshData("data/stones.ant",1.4); 

    //    scene.addNode(new Mesh(*workshop,AGVector4(0,0,0,0),0));
    
    scene.addNode(new TerrainMesh(map));
    
    scene.addNode(new GLTree(AGVector4(3,7,0)));
    
    scene.addNode(new GLTree(AGVector4(4,7,0)));
    scene.addNode(new GLTree(AGVector4(3,8,0)));
    scene.addNode(new GLTree(AGVector4(4,8,0)));
    
    //scene.addNode(new Mesh(*workshop,AGVector4(0,0,0,0),0));
    //    scene.addNode(new Mesh(*workshop,AGVector4(2,2,map.getHeight(2,2),0),-15.0f));
    scene.addNode(new Mesh(*stones,AGVector4(2,2,map.getHeight(2,2),0),0));
    scene.addNode(new Mesh(*farm,AGVector4(10,4,map.getHeight(10,4),0),-15.0f));
    //    scene.addNode(new Mesh(*tree,AGVector4(0.7+1.5,0.2+1.5,0.14,0),-15.0f));
    //    scene.addNode(new Mesh(*tree,AGVector4(0.6+1.5,0.5+1.5,0.14,0),0));
    int i=0;
        for(float x=8;x<15;x+=3)
          for(float y=8;y<15;y+=3)
	{
	  //  float x=0.2,y=0.5;
	  scene.addNode(new Mesh(*man,AGVector4(x,y,map.getHeight(x,y),0),0));
	  i++;
	}
    cdebug("MAN:"<<i);
  }
};

int main()
{
  initRand();
  int w=1024,h=768;
  TestApp app(w,h);

  AGButton b(0,AGRect(0,0,50,50),"test");

  app.setMainWidget(&b);

  PBuffer buffer(1024,1024);
  app.run();
};
