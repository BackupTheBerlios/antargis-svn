#include "ant_renderer.h"
#include "scene.h"
#include "smoke.h"
#include "ag_texturecache.h"
#include "ag_rendercontext.h"
#include "ag_main.h"
#include "map.h"
#include "ag_config.h"
#include "ag_profiler.h"

bool gSmoke;
long lastSmokeTest;

// small caching fct, so that lookup is not that long???
bool smokeEnabled()
{
  long ts=getMain()->getTicks();
  if(getMain()->getTicks()-lastSmokeTest>1000)
    {
      gSmoke=getConfig()->get("particleEffects")!="false";
      lastSmokeTest=ts;
    }
  return gSmoke;
}

float randf()
{
  return (rand()%10000)/10000.0;
}

Smoke::Smoke(Scene *pScene,float f):
  SceneNode(pScene,AGVector4(),AGBox3(AGVector3(-0.5,-0.5,0),AGVector3(1,1,10)))
{
  maxtime=20;

  mtime=0;
  freq=f;
  stime=1/freq;
  mTexture=getTextureCache()->get("data/textures/point.png");
  fire=false;
  setOrder(PARTICLE_Z);
  enabled=true;
}

void Smoke::setFire(bool f)
{
  fire=f;
}

void Smoke::setMaxTime(float m)
{
  maxtime=m;
}

void Smoke::advance(float time)
{
  STACKTRACE;
  if(!smokeEnabled())
    return;
  //  return;
  // advance pieces
  for(std::list<Piece*>::iterator i=mPieces.begin();i!=mPieces.end();i++)
    {
      (*i)->lived+=time;
      // move
      (*i)->pos+=(*i)->speed*time;
    }
  // delete old pieces
  while(mPieces.size())
    {
      if(mPieces.back()->lived>maxtime)
	{
	  delete mPieces.back();
	  mPieces.pop_back();
	}
      else
	break;
    }

  if(!enabled)
    return;

  // create new piece
  mtime+=time;

  float timevar=1;

  if(mtime>stime)
    {
      Piece *ps=new Piece;

      if(fire)
	{
	  ps->pos=getPos().dim3();
	  ps->lived=-timevar*randf();
	  ps->light=randf()*0.8+0.2;//5+0.5;
	  ps->size=randf()*0.05+0.1;
	  ps->speed=AGVector3(randf()*0.4-0.2,0,1+randf()*0.5);
	  ps->color=AGVector3(1,1,0);
	}
      else
	{
	  ps->pos=getPos().dim3();
	  ps->lived=-timevar*randf();
	  ps->light=randf()*0.2+0.2;//5+0.5;
	  ps->size=randf()*0.1+0.2;
	  ps->speed=AGVector3(randf()*0.01,0,0.2+randf()*0.1);
	  ps->color=AGVector3(1,1,1);
	}
      mPieces.push_front(ps);
      mtime-=stime;
	  
    }
}

void Smoke::draw()
{
  STACKTRACE;

  if(!smokeEnabled())
    return;

  AGVector3 dir=getRenderer()->getCurrentScene()->getCameraDirTo(getPos().dim3());

  AGVector3 pdown(0,0,-1);
  AGVector3 side=dir%pdown;
  AGVector3 up=side%dir;

  side.normalize();
  up.normalize();

  AGRenderContext c;
  c.setAlpha(0,GL_NONE);
  c.setDepthWrite(false);
  c.setTexture(mTexture.glTexture());
  c.setColor(AGVector4(1,1,1,1));
  c.begin();

  glBegin(GL_QUADS);

  float a,s,l;
  AGVector4 color;

  for(std::list<Piece*>::iterator i=mPieces.begin();i!=mPieces.end();i++)
    {
      a=1;
      if(!fire)
	if((*i)->lived<1)
	  a=(*i)->lived;
      
      a=std::min(maxtime-(*i)->lived,a);

      l=(*i)->light;
      s=(*i)->size;
      color=AGVector4((*i)->color[0]*l,(*i)->color[1]*l,(*i)->color[2]*l,a);
      if(fire)
	{
	  float x=maxtime/(*i)->lived;
	  x=std::max(std::min(x,1.0f),0.0f);
	  (*i)->color=AGVector3(1,0,0)*x+AGVector3(1,1,0)*(1-x);
	}
      else
	{
	  if((*i)->lived<3)
	    s*=(*i)->lived/3;
	}
      glColor4fv(color);
      glTexCoord2f(0,0);
      glVertex3fv((*i)->pos+(AGVector3(0,0,0)-side+up)*s);

      glColor4fv(color);
      glTexCoord2f(1,0);
      glVertex3fv((*i)->pos+(side+up)*s);

      glColor4fv(color);
      glTexCoord2f(1,1);
      glVertex3fv((*i)->pos+(side-up)*s);
      
      glColor4fv(color);
      glTexCoord2f(0,1);
      glVertex3fv((*i)->pos+(AGVector3(0,0,0)-side-up)*s);

    }
  glEnd();
  
}

void Smoke::setEnabled(bool f)
{
  enabled=f;
}