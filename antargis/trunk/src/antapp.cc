#include "antapp.h"

std::ofstream out("log.txt");

GLApp::GLApp(int w,int h):scene(w,h)
{
  setupLight();
  
  
  shadow=true;
  mx=my=0;
  omx=-1;
  frameTime=0;
  frameCount=0;
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
  
  scene.draw();
  
  glColor4f(1,1,1,1);
  glBegin(GL_LINES);
  glVertex3fv(line.getV0());
  glVertex3fv(line.getV1());
  glEnd();
  
}

bool GLApp::eventFrame(float t)
{
  frameTime+=t;
  frameCount++;
  if(frameCount==10)
    {
      frameTime*=0.1f;
      float fps=1/frameTime;
      out<<fps<<std::endl;
      frameTime=0;
      frameCount=0;
    }
  scene.advance(t);
  SDL_Delay(30);
  return true;
}

bool GLApp::eventMouseButtonDown(const AGEvent *m)
{
  mMayClick=true;
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
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
	  AGLine3 l=scene.getLine(p.x,p.y);
	  cdebug(l.toString());
	  Scene::PickResult nodes=scene.lineHit(l);
	  line=l;
	  //	  eventClick(nodes);
	}
    }
  return AGApplication::eventMouseButtonDown(m);
}
bool GLApp::eventMouseButtonUp(const AGEvent *m)
{
  omx=-1;
  omy=-1;
  if(mMayClick)
    {
      const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
      if(e)
	{
	  AGPoint p=e->getMousePosition();
	  AGLine3 l=scene.getLine(p.x,p.y);
	  cdebug(l.toString());
	  Scene::PickResult nodes=scene.lineHit(l);
	  line=l;

	  for(Scene::PickResult::iterator i=nodes.begin();i!=nodes.end();i++)
	    cdebug("PICKed:"<<&(*i));

	  eventClick(nodes,e->getButton());
	}
    }
  return AGApplication::eventMouseButtonUp(m);
}

void GLApp::eventClick(const Scene::PickResult &pNodes,int button)
{
  cdebug(pNodes.size());
  for(Scene::PickResult::const_iterator i=pNodes.begin();i!=pNodes.end();i++)
    {
      cdebug(i->camDist<<"  "<<i->pos.toString()<<"  "<<typeid(*i->node).name());
    }
}


bool GLApp::eventMouseMotion(const AGEvent *m)
{
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e && omx>=0)
    {
      AGPoint p=e->getMousePosition();
      mx+=p.x-omx;
      my+=p.y-omy;
      
      camera=AGVector4(-mx*0.03,my*0.03,0);
      scene.setCamera(camera);
      
      omx=p.x;
      omy=p.y;
    }
  mMayClick=false;
  return AGApplication::eventMouseMotion(m);
}

bool GLApp::eventKeyDown(const AGEvent *m)
{
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
    {
      if(e->getKey()==SDLK_s)
	{
	  //	    shadow=!shadow;
	  scene.setShadow((scene.getShadow()+1)%3);
	}
    }
  return AGApplication::eventKeyDown(m);
}

// day light
void GLApp::setupLight()
{
  GLfloat pos[] = { -2.0, 0.0, 3.1, 0.0};
  GLfloat col[] = {1.0, 1.0, 1.0, 1.0};
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0,GL_POSITION, pos);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,  col);
  glLightfv(GL_LIGHT0,GL_SPECULAR, col);
  
  glEnable(GL_LIGHTING);
}

Scene &GLApp::getScene()
{
      return scene;
    }

void GLApp_markfunc(void *ptr)
{
  if(!ptr)
    return;

  GLApp *zoo;

  zoo = static_cast<GLApp*>(ptr);
  
  if(zoo->scene.mRubyObject)
    rb_gc_mark(zoo->scene.mRUBY);

}
