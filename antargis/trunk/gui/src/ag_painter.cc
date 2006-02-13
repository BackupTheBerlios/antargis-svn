/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_painter.cc
 * by David Kamphausen (david.kamphausen@web.de)
 *
 * The "Antargis" project, including all files needed to compile it,
 * is free software; you can redistribute it and/or use it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 */

#include "ag_painter.h"
#include "ag_debug.h"
#include "ag_texture.h"
#include "ag_draw.h"
#include "ag_fontengine.h"
#include "ag_glsurface.h"
#include <math.h>

//#define SPEED_TEST

/////////////////////////////////////////////////////////////////////////////////
// AGProjection
/////////////////////////////////////////////////////////////////////////////////


AGProjection::AGProjection(const AGRect2 &pClip):clip(pClip)
{
  a.set(0,0,1);
  a.set(0,1,0);
  a.set(0,2,0);
  a.set(1,0,0);
  a.set(1,1,1);
  a.set(1,2,0);
  a.set(2,0,0);
  a.set(2,1,0);
  a.set(2,2,0);
}

AGPoint AGProjection::project(const AGPoint &p) const
{
  AGPoint r=(a*AGVector3(p[0],p[1],1)).dim2();

  return r;
}
bool AGProjection::pointOk(const AGPoint &p) const
{
  return clip.contains(p);
}

AGRect AGProjection::project(const AGRect &p) const
{
  AGRect2 r((a*AGVector3(p[0],1)).dim2(),
	    (a*AGVector3(p[1],1)).dim2());
  return r;
}
std::pair<AGRect,AGRect> AGProjection::clipRect(AGRect target,AGRect src) const
{
  // clip left
  if(target.x0()<clip.x0())
    {
      float a=(clip.x0()-target.x0())/target.w();
      target.setLeft(clip.x0());
      src.setLeft(src.x0()+src.w()*a);
    }

  // clip top
  if(target.y0()<clip.y0())
    {
      float a=(clip.y0()-target.y0())/target.h();
      target.setTop(clip.y0());
      src.setTop(src.y0()+src.h()*a);
    }

  // clip right
  if(target.x1()>clip.x1())
    {
      float a=(target.x1()-clip.x1())/target.w();
      target.setRight(clip.x1());
      src.setRight(src.x1()-src.w()*a);
    }

  // clip bottom
  if(target.y1()>clip.y1())
    {
      float a=(target.y1()-clip.y1())/target.h();
      target.setBottom(clip.y1());
      src.setBottom(src.y1()-src.h()*a);
    }

  return std::make_pair(target,src);
}

void AGProjection::translate(const AGVector2 &v)
{
  a.get(2,0)+=v[0];
  a.get(2,1)+=v[1];
}

void AGProjection::setClip(const AGRect2&p)
{
  clip=clip.intersect(p);
}


AGRect2 AGProjection::getRect() const
{
  AGRect r=clip;

  r-=AGVector2(a.get(2,0),a.get(2,1));
  return r;
}

AGLine2 AGProjection::clipLine(AGLine2 l) const
{
  AGLine2 d;

  AGRect r=clip;

  if((l[0][0]<r.x0() && l[1][0]<r.x0()) ||
     (l[0][0]>r.x1() && l[1][0]>r.x1()) ||
     (l[0][1]<r.y0() && l[1][1]<r.y0()) ||
     (l[0][1]>r.y1() && l[1][1]>r.y1()))
    return d; // hidden

  float a;
  // clip
  for(size_t i=0;i<1;i++)
    {
      if(l[i][0]<r.x0())
	{
	  a=(r.x0()-l[i][0])/(l[1-i][0]-l[i][0]);
	  l[i][0]=r.x0();
	  l[i][1]=l[i][1]+(l[1-i][1]-l[i][1])*a;
	}
      if(l[i][1]<r.y0())
	{
	  a=(r.y0()-l[i][1])/(l[1-i][1]-l[i][1]);
	  l[i][1]=r.y0();
	  l[i][0]=l[i][0]+(l[1-i][0]-l[i][0])*a;
	}
      if(l[i][0]>r.x1())
	{
	  a=(l[i][0]-r.x1())/(l[i][0]-l[1-i][0]);
	  l[i][0]=r.x1();
	  l[i][1]=l[i][1]+(l[1-i][1]-l[i][1])*a;
	}
      if(l[i][1]>r.y1())
	{
	  a=(l[i][1]-r.y1())/(l[i][1]-l[1-i][1]);
	  l[i][1]=r.y1();
	  l[i][0]=l[i][0]+(l[1-i][0]-l[i][0])*a;
	}

    }
  return l;
    
}



/////////////////////////////////////////////////////////////////////////////////
// AGPainter
/////////////////////////////////////////////////////////////////////////////////

AGPainter::AGPainter():mCurrent(getScreen().getRect()),mTarget(&getScreen())
{
  mTarget->beginPaint();
}

AGPainter::AGPainter(const AGPainter &p):ps(p.ps),mCurrent(p.mCurrent),mTarget(p.mTarget)
{
  mTarget->beginPaint();
}

AGPainter::AGPainter(AGPaintTarget &pTarget):mCurrent(pTarget.getRect()),mTarget(&pTarget)
{
  mTarget->beginPaint();
}


AGPainter::~AGPainter()
{
  mTarget->endPaint();
}

void AGPainter::putPixel(const AGPoint &p,const AGColor &c)
{
  AGPoint n=project(p);
  if(pointOk(n))
    mTarget->putPixel((int)n[0],(int)n[1],c);
}

void AGPainter::blit(const AGTexture &pSource,const AGRect &pDest)
{
  AGRect s=pSource.getRect();
  // ASSUME: we don't want to scale
  blit(pSource,AGRect(pDest.x(),pDest.y(),s.w(),s.h()),s);
}

AGPoint AGPainter::project(const AGPoint &p) const
{
  return mCurrent.project(p);
}
bool AGPainter::pointOk(const AGPoint &p) const
{
  return mCurrent.pointOk(p);
}


void AGPainter::blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  AGRect2 d;
  d=mCurrent.project(pDest);
  std::pair<AGRect,AGRect> p=mCurrent.clipRect(d,pSrc);

  if(p.first.w()>0 && p.first.h()>0 && p.second.w()>0 && p.second.h()>0)
    mTarget->blit(pSource,p.first,p.second);
}

void AGPainter::blit(const AGTexture &pSource,const AGRect &pDest,const AGColor &pColor)
{
  AGRect s=pSource.getRect();
  // ASSUME: we don't want to scale
  blit(pSource,AGRect(pDest.x(),pDest.y(),s.w(),s.h()),s,pColor);
}

void AGPainter::blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc,const AGColor &pColor)
{
#ifdef SPEED_TEST
  mTarget->blit(pSource,pDest,pSrc,pColor);
  return;
#endif

  AGRect2 d;
  d=mCurrent.project(pDest);
  std::pair<AGRect,AGRect> p=mCurrent.clipRect(d,pSrc);

  if(p.first.w()>0 && p.first.h()>0 && p.second.w()>0 && p.second.h()>0)
    mTarget->blit(pSource,p.first,p.second,pColor);
}


void AGPainter::tile(const AGTexture &pSource)
{
  tile(pSource,mTarget->getRect());
}
void AGPainter::tile(const AGTexture &pSource,const AGRect &pDest)
{
  tile(pSource,pDest,pSource.getRect());
}
void AGPainter::tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  //  return;
  /*  AGGLScreen *glScreen=dynamic_cast<AGGLScreen*>(mTarget);
  if(glScreen && false)
    {
      AGRect d=mCurrent.project(pDest);
      //      glScreen->clip(mCurrent.clip);
      glScreen->tile(pSource,d,pSrc);
      glScreen->unclip();
      return;
      }*/
  float x,y;
  for(y=pDest.y0();y<pDest.y1();y+=pSrc.h())
    for(x=pDest.x0();x<pDest.x1();x+=pSrc.w())
      {
	float w=std::min(pSrc.w(),pDest.x1()-x);
	float h=std::min(pSrc.h(),pDest.y1()-y);
	blit(pSource,AGRect(x,y,w,h),AGRect(pSrc.x0(),pSrc.y0(),w,h));
	//	return;
      }
}


// AGSurface-painting
void AGPainter::blit(const AGSurface &pSource,const AGRect &pDest)
{
  AGRect s=pSource.getRect();
  // ASSUME: we don't want to scale
  blit(pSource,AGRect(pDest.x(),pDest.y(),s.w(),s.h()),s);
}
void AGPainter::blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  AGRect2 d;
  d=mCurrent.project(pDest);
  std::pair<AGRect,AGRect> p=mCurrent.clipRect(d,pSrc);

  if(p.first.w()>0 && p.first.h()>0 && p.second.w()>0 && p.second.h()>0)
    mTarget->blit(pSource,p.first,p.second);
}
void AGPainter::tile(const AGSurface &pSource)
{
  tile(pSource,mTarget->getRect());
}
void AGPainter::tile(const AGSurface &pSource,const AGRect &pDest)
{
  tile(pSource,pDest,pSource.getRect());

}
void AGPainter::tile(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  float x,y;
  for(y=pDest.y0();y<pDest.y1();y+=pSrc.h())
    for(x=pDest.x0();x<pDest.x1();x+=pSrc.w())
      {
	float w=std::min(pSrc.w(),pDest.x1()-x);
	float h=std::min(pSrc.h(),pDest.y1()-y);
	blit(pSource,AGRect(x,y,w,h),AGRect(pSrc.x0(),pSrc.y0(),w,h));
      }
}

AGColor calcColor(AGPoint p,const AGColor &pc0,const AGColor &pc1,const AGColor &pc2,const AGColor &pc3)
{
  return (pc0*(1-p[0]) + pc1*p[0])*(1-p[1]) + (pc2*(1-p[0]) + pc3*p[0])*p[1];
}
  
void AGPainter::drawGradient(const AGRect &pr,const AGColor &pc0,const AGColor &pc1,const AGColor &pc2,const AGColor &pc3)
{
  AGGLScreen *glScreen=dynamic_cast<AGGLScreen*>(mTarget);

  AGRect src(0,0,1,1);

  AGRect d=mCurrent.project(pr);
  std::pair<AGRect,AGRect> p=mCurrent.clipRect(d,src);

  AGColor c0=calcColor(p.second.getV0(),pc0,pc1,pc2,pc3);
  AGColor c1=calcColor(p.second.getV10(),pc0,pc1,pc2,pc3);
  AGColor c2=calcColor(p.second.getV01(),pc0,pc1,pc2,pc3);
  AGColor c3=calcColor(p.second.getV1(),pc0,pc1,pc2,pc3);

  AGRect r=p.first;

  if(p.first.w()>0 && p.first.h()>0 && p.second.w()>0 && p.second.h()>0)
    {
      if(glScreen)
	glScreen->drawGradient(r,c0,c1,c2,c3);
      else
	{
	  float cx,cy,y,x;
	  float dx=1.0/r.w();
	  float dy=1.0/r.h();
	  for(cy=0,y=r.y0();y<r.y1();y+=1,cy+=dy)
	    for(cx=0,x=r.x0();x<r.x1();x+=1,cx+=dx)
	      putPixel(AGPoint(x,y),(c0*(1-cx)+c1*cx)*(1-cy)+(c2*(1-cx)+c3*cx)*cy);
	}
    }
}
void AGPainter::renderText(const std::string &pText,const AGPoint &p,const AGFont &f)
{
  AGTexture *t=AGFontEngine::renderText(0,0,pText,f);
  if(t)
    blit(*t,AGRect(p[0],p[1],t->width(),t->height()));

}
void AGPainter::drawBorder(const AGRect& pRect,int width, const AGColor& c1, const AGColor& c2)
{
  AGGLScreen *glScreen=dynamic_cast<AGGLScreen*>(mTarget);
  if(glScreen)
    {
      #warning "add clipping in gl"
      glScreen->clip(mCurrent.clip);
      glScreen->drawBorder(mCurrent.project(pRect),width,c1,c2);
      //      glScreen->fillRect(AGRect(0,0,1024,768),c1);
      glScreen->unclip();
      return;
    }
  // FIXME: 
  // 1) maybe improve gl-renderer by using triangles
  // 2) otherwise use line-drawing (which is clipped and transformed itself
  // 3) transform is done for each line - maybe is slow
  AGRect d=pRect;

  for(int t=0;t<width;t++)
    {
      drawLine(d.getV0(),d.getV01(),c1);
      drawLine(d.getV0(),d.getV10(),c1);

      drawLine(d.getV01(),d.getV1(),c2);
      drawLine(d.getV10(),d.getV1(),c2);
      d=d.shrink(1);
    }
}

void AGPainter::fillRect(const AGRect &pDest,const AGColor &c)
{
  AGRect2 d,pSrc;
  d=mCurrent.project(pDest);
  std::pair<AGRect,AGRect> p=mCurrent.clipRect(d,pSrc);

  if(p.first.w()>0 && p.first.h()>0)
    mTarget->fillRect(p.first,c);
}

AGColor AGPainter::getPixel(int x,int y)
{
  AGPoint p=mCurrent.project(AGPoint(x,y));
  if(!mCurrent.pointOk(p))
    throw std::string("pixel out of clipping!");
  return mTarget->getPixel((int)p[0],(int)p[1]);
}

void AGPainter::drawLine(const AGPoint &p0,const AGPoint &p1,const AGColor &c)
{
  #warning "Add line clipping"
  AGVector2 m0=mCurrent.project(p0);
  AGVector2 m1=mCurrent.project(p1);
  mTarget->drawLine(m0,m1,c);
}

void AGPainter::blitTri(const AGTexture &pSource,const AGTriangle2 &pSrc,const AGTriangle2 &pDest)
{
  mTarget->blitTri(pSource,pSrc,pDest);
}

void AGPainter::drawCircle(const AGPoint &p,float rad,const AGColor &color)
{
  // clipping done in putPixel

  int x,y;
  float r2=rad*rad;
  for(x=((int)(p[0]-rad-1));x<=p[0]+rad+1;x++)
    for(y=((int)(p[1]-rad-1));y<=p[1]+rad+1;y++)
      {
	float dx=x-p[0];
	float dy=y-p[1];
	float d2=dx*dx+dy*dy;

	if(d2<r2)
	  {
	    AGPoint c(x,y);
	    putPixel(c,color);
	  }
      }
}

void AGPainter::pushMatrix()
{
  ps.push_back(mCurrent);
}
void AGPainter::popMatrix()
{
  mCurrent=ps.back();
  ps.pop_back();
}
void AGPainter::translate(const AGVector2 &v)
{
  mCurrent.translate(v);
}
void AGPainter::scale(const AGVector2 &v)
{
  throw std::string("not implemented!");
}
void AGPainter::clip(const AGRect2 &r)
{
  AGRect p=mCurrent.project(r);
  mCurrent.setClip(p);
}

void AGPainter::transform(const AGRect2 &r)
{
  translate(r[0]);
  clip(r.origin());
}

AGRect AGPainter::getRect() const
{
  return mCurrent.getRect();
}
