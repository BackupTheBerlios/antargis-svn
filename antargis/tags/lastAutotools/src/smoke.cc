#include "renderer.h"
#include "scene.h"
#include "smoke.h"
#include "ag_texturecache.h"
#include "map.h"

float randf()
{
  return (rand()%10000)/10000.0;
}

Smoke::Smoke(float f)
{
  maxtime=20;

  mtime=0;
  freq=f;
  stime=1/freq;
  p=AGVector3(0,0,0);
  mTexture=getTextureCache()->get("data/textures/point.png");
  fire=false;
  setOrder(PARTICLE_Z);
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

  // create new piece
  mtime+=time;

  float timevar=1;

  if(mtime>stime)
    {
      Piece *ps=new Piece;

      if(fire)
	{
	  ps->pos=p;
	  ps->lived=-timevar*randf();
	  ps->light=randf()*0.8+0.2;//5+0.5;
	  ps->size=randf()*0.05+0.1;
	  ps->speed=AGVector3(randf()*0.4-0.2,0,1+randf()*0.5);
	  ps->color=AGVector3(1,1,0);
	}
      else
	{
	  ps->pos=p;
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
  //  return;
  glDisable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glDepthMask(false);

  AGVector3 dir=getRenderer()->getCurrentScene()->getCameraDirTo(p);

  //  cdebug(dir);

  AGVector3 pdown(0,0,-1);
  AGVector3 side=dir%pdown;
  AGVector3 up=side%dir;

  side.normalize();
  up.normalize();


  for(std::list<Piece*>::iterator i=mPieces.begin();i!=mPieces.end();i++)
    {
      float a=1;
      if(!fire)
	if((*i)->lived<1)
	  a=(*i)->lived;
      
      a=std::min(maxtime-(*i)->lived,a);

      glBindTexture(GL_TEXTURE_2D,mTexture.getTextureID());

      float l=(*i)->light;
      float s=(*i)->size;
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

      glColor4f((*i)->color[0]*l,(*i)->color[1]*l,(*i)->color[2]*l,a);
      glBegin(GL_QUADS);
      glTexCoord2f(0,0);
      glVertex3fv((*i)->pos+(AGVector3(0,0,0)-side+up)*s);
      glTexCoord2f(1,0);
      glVertex3fv((*i)->pos+(side+up)*s);
      glTexCoord2f(1,1);
      glVertex3fv((*i)->pos+(side-up)*s);
      glTexCoord2f(0,1);
      glVertex3fv((*i)->pos+(AGVector3(0,0,0)-side-up)*s);
      glEnd();
    }
  glDepthMask(true);
  
}


void Smoke::setPos(const AGVector3&pPos)
{
  p=pPos;  
  mBBox=AGBox3(p+AGVector3(-0.5,-0.5,0),AGVector3(1,1,10));
}

AGBox3 Smoke::bbox() const
{
  return mBBox;
}
