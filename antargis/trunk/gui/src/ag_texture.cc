/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_texture.cc
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

#include "ag_texture.h"
#include "ag_glsurface.h"
#include "ag_debug.h"
#include "ag_rendercontext.h"
#include "sge.h"
#include "ag_surfacemanager.h"
#include <stdexcept>

size_t nextpow2(size_t i)
{
  size_t j=1;
  while(j<i)
    j<<=1;

  return j;
}


///////////////////////////////////////////////////////////
// AGTexture
///////////////////////////////////////////////////////////


AGTexture::AGTexture(const AGTexture &t)
{
  mSDLTexture=t.mSDLTexture;
  mTexture=t.mTexture;

  mTextureUsed=false;
  mPainting=false;
  m3d=t.m3d;
  s=t.s;

  w=t.w;
  h=t.h;
  getSurfaceManager()->registerMe(this);
}

AGTexture::AGTexture()
{
  w=h=0;
  s=0;
  m3d=false;
  mTexture=0;
  mSDLTexture=0;
  mTextureUsed=false;
  getSurfaceManager()->registerMe(this);
  mPainting=false;
}

AGTexture::AGTexture(int W,int H):w(W),h(H),s(0)
{
  m3d=0;
  mTexture=0;
  mSDLTexture=0;
  mTextureUsed=false;
  mPainting=false;
  getSurfaceManager()->registerMe(this);
}



AGTexture::AGTexture(const AGSurface &pSurface, bool p3d)
{
  w=pSurface.width();
  h=pSurface.height();
  m3d=p3d;

  mTextureUsed=false;
  mTexture=0;
  mSDLTexture=0;

  s=pSurface.surface();

  getSurfaceManager()->registerMe(this);
  mPainting=false;
}


AGTexture::~AGTexture()
{
  if(getSurfaceManager())
    getSurfaceManager()->deregisterMe(this);
}

int AGTexture::width() const
{
  return w;
}
int AGTexture::height() const
{
  return h;
}

AGRect AGTexture::getRect() const
{
  return AGRect(0,0,w,h);
}

AGColor AGTexture::getPixel(int x,int y) const
{
  assert(!opengl());
  Uint32 c=sge_GetPixel(s->surface,x,y);
  Uint8 r,g,b,a;
  SDL_GetRGBA(c,s->surface->format,&r,&g,&b,&a);
  return AGColor(r,g,b,a);
}

bool AGTexture::hasTexture() const
{
  return mTexture;
}
void AGTexture::clearTexture()
{
  mTexture=0;
}
void AGTexture::clearTextureUsed()
{
  mTextureUsed=false;
}

AGInternalSurface *AGTexture::sdlTexture()
{
  assert(!opengl());
  if(mSDLTexture==0)
    {
      mSDLTexture=new AGInternalSurface;
      mSDLTexture->surface=SDL_DisplayFormatAlpha(s->surface);
      s->sdlTexture=mSDLTexture;
    }
  return mSDLTexture;
}


AGGLTexture *AGTexture::glTexture()
{
  assert(opengl());
  if(mTexture==0)
    {
      if(s)
	if(s->glTexture)
	  mTexture=s->glTexture;
      if(!mTexture)
	{
	  if(m3d)
	    mTexture=new AGGLTexture(nextpow2(std::min(w,h)),nextpow2(std::min(w,h)),nextpow2(std::max(w,h)/std::min(w,h)));
	  else
	    mTexture=new AGGLTexture(nextpow2(w),nextpow2(h));
	  if(s)
	    {
	      mTexture->setSurface(s);
	      s->glTexture=mTexture;
	    }
	}
    }
  mTextureUsed=true;
  return mTexture;
}

bool AGTexture::textureUsed() const
{
  return mTextureUsed;
}

/*
AGInternalSurface *AGTexture::surface()
{
  return s;
  }*/

void AGTexture::beginPaint()
{
  assert(!isRendering());
  assert(!m3d);
  if(opengl())
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
      // init 2d drawing
      getScreen().begin();
      // copy texture to buffer
      AGRenderContext c;
      c.setColor(AGVector4(1,1,1,1));
      c.setTexture(glTexture());
      c.setCulling(false);
      c.begin();
      //      bindTexture();
      
      float w=mTexture->width(),h=mTexture->height();
      //      glDisable(GL_CULL_FACE);
      //      glColor4f(1,1,1,1);
      glBegin(GL_QUADS);
      glTexCoord2f(0,0);
      glVertex2f(0,0);

      glTexCoord2f(1,0);
      glVertex2f(w,0);

      glTexCoord2f(1,1);
      glVertex2f(w,h);

      glTexCoord2f(0,1);
      glVertex2f(0,h);
      glEnd();
      
      glMatrixMode(GL_MODELVIEW);
      glScalef(1,-1,1);
      glTranslatef(0,h,0);

    }
  mPainting=true;
}
void AGTexture::endPaint()
{
  if(opengl())
    {
      bindTexture();
      
      // copy buffer to texture

      glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, mTexture->width(),mTexture->height(),0);

      getScreen().flip();
      SDL_Delay(1000);


    }
  mPainting=false;
}

void AGTexture::putPixel(int x,int y,const AGColor &c)
{
  assert(mPainting);
  if(opengl())
    {
      assert(mTexture);
      // use screen-functions
      y=(mTexture->height()-1)-y;
      getScreen().putPixel(x,(getScreen().getHeight()-1)-y,c);
    }
  else
    {
      sge_PutPixel(s->surface,x,y,c.mapRGB(s->surface->format));
    }
}


void AGTexture::fillRect(const AGRect &pRect,const AGColor &c)
{
  if(opengl())
    {
      assert(mTexture);
      cdebug(pRect);
      getScreen().fillRect(pRect,c);
    }
  else
    throw std::runtime_error("implement fillRect for sdl-texture");
  
}

void AGTexture::blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  if(opengl())
    {
      assert(mTexture);
      getScreen().blit(pSource,pDest,pSrc);
    }
  else
    throw std::runtime_error("implement blitting for sdl-texture");
}
void AGTexture::blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc,const AGColor &pColor)
{
  if(opengl())
    {
      assert(mTexture);
      getScreen().blit(pSource,pDest,pSrc,pColor);
    }
  else
    throw std::runtime_error("implement blitting for sdl-texture");
}


void AGTexture::setWrapping(bool pWrap)
{
  bindTexture();
  if(m3d && pWrap)
    setClamp(GL_REPEAT,GL_REPEAT,GL_CLAMP_TO_EDGE );
}
void AGTexture::setFilter(GLuint mag,GLuint min)
{
  glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, mag);
  glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, min);
}

void AGTexture::bindTexture()
{
  AGRenderContext c;
  c.setTexture(glTexture());
  c.begin();
}

float AGTexture::getTW() const
{
  if(mTexture)
    return float(w)/float(mTexture->width());
  return 1;
}
float AGTexture::getTH() const
{
  if(mTexture)
    return float(h)/float(mTexture->height());
  return 1;
}

float AGTexture::getSurfaceWidth() const
{
  return float(w);
}
float AGTexture::getSurfaceHeight() const
{
  return float(h);
}

float AGTexture::getTextureWidth() const
{
  return float(mTexture->width());
}
float AGTexture::getTextureHeight() const
{
  return float(mTexture->height());
}

bool AGTexture::is3d() const
{
  return m3d;
}

void AGTexture::setClamp(GLuint s,GLuint t,GLuint r)
{
  return;
  bindTexture();
  glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, s);
  glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, t);
  glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, r);
}
