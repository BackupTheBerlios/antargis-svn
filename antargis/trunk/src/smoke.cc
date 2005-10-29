#include "smoke.h"
#include "ag_texturecache.h"
#include "map.h"
#include "scene.h"

float randf()
{
  return (rand()%10000)/10000.0;
}

Smoke::Smoke(const AGVector3 &pStart,float f)
{
  maxtime=20;

  mtime=0;
  freq=f;
  stime=1/freq;
  p=pStart;
  mTexture=getTextureCache()->get("data/textures/smoke3.png");

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
      ps->pos=p;
      ps->lived=-timevar*randf();
      ps->light=randf()*0.5+0.5;
      ps->size=randf()*0.1+0.2;
      ps->speed=AGVector3(randf()*0.01,0,0.2+randf()*0.1);
      mPieces.push_front(ps);
      mtime-=stime;
    }
}

void Smoke::draw()
{
  glDisable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glDepthMask(false);

  AGVector3 dir=getMap()->getScene()->getCameraDirTo(p);

  //  cdebug(dir);

  AGVector3 pdown(0,0,-1);
  AGVector3 side=dir%pdown;
  AGVector3 up=side%dir;

  side.normalize();
  up.normalize();


  for(std::list<Piece*>::iterator i=mPieces.begin();i!=mPieces.end();i++)
    {
      float a;
      if((*i)->lived<1)
	a=(*i)->lived;
      else
	a=std::min(maxtime-(*i)->lived,1.0f);
      glBindTexture(GL_TEXTURE_2D,mTexture.getTextureID());

      float l=(*i)->light;
      float s=(*i)->size;
      if((*i)->lived<3)
	s*=(*i)->lived/3;

      glColor4f(l,l,l,a);
      glBegin(GL_QUADS);
      glTexCoord2f(0,0);
      glVertex3fv((*i)->pos+(AGVector3(0,0,0)-side+up)*s);//AGVector3(-s,0,s));
      glTexCoord2f(1,0);
      glVertex3fv((*i)->pos+(side+up)*s);//AGVector3(s,0,s));
      glTexCoord2f(1,1);
      glVertex3fv((*i)->pos+(side-up)*s);//AGVector3(s,0,-s));
      glTexCoord2f(0,1);
      glVertex3fv((*i)->pos+(AGVector3(0,0,0)-side-up)*s);//AGVector3(-s,0,-s));

      /*
      glTexCoord2f(0,0);
      glVertex3fv((*i)->pos+AGVector3(-s,0,s));
      glTexCoord2f(1,0);
      glVertex3fv((*i)->pos+AGVector3(s,0,s));
      glTexCoord2f(1,1);
      glVertex3fv((*i)->pos+AGVector3(s,0,-s));
      glTexCoord2f(0,1);
      glVertex3fv((*i)->pos+AGVector3(-s,0,-s));
      */
      glEnd();
    }
  glDepthMask(true);
}


void Smoke::setPos(const AGVector3&pPos)
{
  p=pPos;  
}
