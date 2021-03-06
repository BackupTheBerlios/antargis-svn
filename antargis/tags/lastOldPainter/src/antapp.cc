#include "antapp.h"
#include "ag_debug.h"

std::ofstream out("log.txt");

GLApp::GLApp(int w,int h):scene(w,h)
{
  shadow=true;
  mx=my=0;
  omx=-1;
  frameTime=0;
  frameCount=0;
  hx=hy=-1;
}

GLApp::~GLApp()
{
  CTRACE;
}


void GLApp::draw()
{
  drawGL();
  AGApplication::draw();
}

void GLApp::drawGL()
{
  glEnable(GL_DEPTH_TEST); // enable depth test
  glDepthMask(true);

  assertGL;
  scene.draw();
  assertGL;
  
  glColor4f(1,1,1,1);
}

bool GLApp::eventFrame(float t)
{
  if(hx>=0)
    {
      // check hovering
      Scene::PickResult nodes=scene.pick(hx,hy,1,1);
      
      if(nodes.size())
	eventHover(nodes,hb);
      hx=hy=-1;
    }
  return true;
}

bool GLApp::eventMouseButtonDown(AGEvent *e)
{
  mMayClick=true;

  if(e->isSDLEvent())
    {
      if(e->getButton()==3)
	{
	  AGPoint p=e->getMousePosition();
	  omx=p.x;
	  omy=p.y;
	}
      else if(e->getButton()==1)
	{
	  AGPoint p=e->getMousePosition();
	  Scene::PickResult nodes=scene.pick(p.x,p.y,1,1);
	}
    }
  return AGApplication::eventMouseButtonDown(e);
}
bool GLApp::eventMouseButtonUp(AGEvent *e)
{
  omx=-1;
  omy=-1;
  if(mMayClick)
    {
      if(e->isSDLEvent())
	{
	  AGPoint p=e->getMousePosition();
	  Scene::PickResult nodes=scene.pick(p.x,p.y,1,1);

	  eventClick(nodes,e->getButton());
	}
    }
  return AGApplication::eventMouseButtonUp(e);
}

void GLApp::eventClick(const Scene::PickResult &pNodes,int button)
{
}

void GLApp::eventHover(const Scene::PickResult &pNodes,int button)
{
}


bool GLApp::eventMouseMotion(AGEvent *e)
{
  if(e->isSDLEvent() && omx>=0)
    {
      AGPoint p=e->getMousePosition();
      //      AGVector4 cam=scene.getCamera();
      mx=p.x-omx;
      my=p.y-omy;
      
      setCamera(getCamera()+AGVector2(-mx*0.03,my*0.03));
      //      camera=cam+AGVector4(-mx*0.03,my*0.03,0);
      //      scene.setCamera(camera);
      
      omx=p.x;
      omy=p.y;
    }
  if(e->isSDLEvent())
    {
      // check hovering - delayed 
      AGPoint p=e->getMousePosition();
      hx=p.x;
      hy=p.y;
      hb=e->getButton();
    }

  mMayClick=false;
  return AGApplication::eventMouseMotion(e);
}

AGVector2 GLApp::getCamera() const
{
  return scene.getCamera().dim2();
}

void GLApp::setCamera(const AGVector2 &p)
{
  scene.setCamera(AGVector4(p[0],p[1],0));
}

bool GLApp::eventKeyDown(AGEvent *e)
{
  if(e->isSDLEvent())
    {
      if(e->getKey()==SDLK_s)
	{
	  //	    shadow=!shadow;
	  scene.setShadow((scene.getShadow()+1)%3);
	}
    }
  return AGApplication::eventKeyDown(e);
}

Scene &GLApp::getScene()
{
  return scene;
}

void GLApp::mark()
{
  markObject(&scene);
}
