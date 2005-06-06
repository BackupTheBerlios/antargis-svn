/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_surface.cc
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

#include <iostream>

#include "ag_surface.h"
#include "ag_color.h"
#include "ag_debug.h"
#include "ag_draw.h"
#include "privates.h"
#include "sge.h"
#include "ag_kill.h"

#include "SDL_image.h"

#include <sstream>

#include <ag_fs.h>

SDL_Surface *AGCreate32BitSurface(size_t width,size_t height)
{
  /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
     as expected by OpenGL for textures */
  SDL_Surface *surface;
  Uint32 rmask, gmask, bmask, amask;

  /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN

  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else

  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
                                 rmask, gmask, bmask, amask);
  if(!surface)
    {
      std::cerr<<"CreateRGBSurface failed: "<<SDL_GetError()<<std::endl;
      //        throw UI::UIException();
    }
  return surface;
}


AGSurface::AGSurface(SDL_Surface *pS):s(pS)
{
  getSurfaceManager()->registerSurface(this);
}

AGSurface *AGSurface::clone() const
{
  return new AGSurface(*this);
}


AGSurface::AGSurface()
{
  s=0;
  getSurfaceManager()->registerSurface(this);
}


bool AGSurface::valid() const
{
  return s;
}

AGSurface::AGSurface(const AGSurface &p)
{
  if(p.valid())
    {
      s=AGCreate32BitSurface(p.width(),p.height());//sge_CreateAlphaSurface(SDL_SWSURFACE,p.width(),p.height());
      assert(s);
      for(int x=0;x<p.width();x++)
	for(int y=0;y<p.height();y++)
	  sge_PutPixel(s,x,y,sge_GetPixel(p.s,x,y));
      //      SDL_BlitSurface(p.s,0,s,0);
    }
  else
    s=0;
  
  // FIXME: this gets called too often!!!
  //  CTRACE;
  getSurfaceManager()->registerSurface(this);
}

AGSurface::AGSurface(int w,int h)
{
  s=AGCreate32BitSurface(w,h);
      //  s=sge_CreateAlphaSurface(SDL_SWSURFACE,w,h);
  //  CTRACE;
  getSurfaceManager()->registerSurface(this);
}

AGSurface::~AGSurface()
{
  getSurfaceManager()->deregisterSurface(this);
}

AGSurface &AGSurface::operator=(const AGSurface &p)
{
  CTRACE;
  //  if(s)
  //    SDL_FreeSurface(s);
  s=0;
  if(p.valid())
    {

      if(true)
	{
      //      s=sge_CreateAlphaSurface(SDL_SWSURFACE,p.width(),p.height());
      s=AGCreate32BitSurface(p.width(),p.height());
      getSurfaceManager()->registerSurface(this);
      
      Uint8 r,g,b,a;
      Uint32 c;
      for(int x=0;x<p.width();x++)
	for(int y=0;y<p.height();y++)
	  {
	    c=sge_GetPixel(p.s,x,y);
	    SDL_GetRGBA(c,p.s->format,&r,&g,&b,&a);

	    sge_PutPixel(s,x,y,SDL_MapRGBA(s->format,r,g,b,a));
	  }
      //      SDL_BlitSurface(p.s,0,s,0);
	}
      else
	{
	  //s=sge_CreateAlphaSurface(SDL_SWSURFACE,p.width(),p.height());
	  s=AGCreate32BitSurface(p.width(),p.height());
	  getSurfaceManager()->registerSurface(this);
	  SDL_BlitSurface(p.s,0,s,0);
	  SDL_Flip(s);
	}
    }
  return *this;
}



void AGSurface::putPixel(int x,int y,const AGColor &c)
{
  //  CTRACE;
  sge_PutPixel(s,x,y,color(c));
}

AGColor AGSurface::getPixel(int x,int y) const
{
  return AGColor(sge_GetPixel(s,x,y),*this);
}



SDL_Surface *AGSurface::surface()
{
  CHECK_ZERO(s);
  return s;
}

 //int k=0;

AGSurface AGSurface::getSubSurface(const AGRect &sr) const
{
  CHECK_ZERO(s);
  CTRACE;
  //  SDL_Surface *ns=getScreen().newSurface(sr.w,sr.h);//sge_CreateAlphaSurface(SDL_SWSURFACE,sr.w,sr.h);
  SDL_Surface *ns=AGCreate32BitSurface(sr.w,sr.h);

  AGRect dr(0,0,sr.w,sr.h);
  Uint32 c;
  Uint8 r,g,b,a;

  SDL_SaveBMP(s,"sub1.bmp");

  cdebug(sr);
  
  for(int x=0;x<sr.w;x++)
    for(int y=0;y<sr.h;y++)
      {
	c=sge_GetPixel(s,x+sr.x,y+sr.y);
	SDL_GetRGBA(c,s->format,&r,&g,&b,&a);
	c=SDL_MapRGBA(ns->format,r,g,b,a);
	
	sge_PutPixel(ns,x,y,c);
	}
  //  SDL_BlitSurface(s,const_cast<AGRect*>(&sr),ns,&dr);

  SDL_SaveBMP(ns,"sub2.bmp");

  return AGSurface(ns);//,sr.w,sr.h);
}

   
Uint32 AGSurface::color(const AGColor &c) const
{
  CHECK_ZERO(s);
  return c.mapRGB(s->format);
}

AGRect AGSurface::getRect() const
{
  CHECK_ZERO(s);
  if(s)
    return AGRect(0,0,s->w,s->h);
  else
    return AGRect(0,0,0,0);
}


int AGSurface::width() const
{
  CHECK_ZERO(s);
  return s->w;
}
int AGSurface::height() const
{
  CHECK_ZERO(s);
  return s->h;
}

void AGSurface::drawGradient(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  CHECK_ZERO(s);
  AGDrawGradientAlpha(s,rect,ul,ur,dl,dr);
}

void AGSurface::drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  CHECK_ZERO(s);
  AGDrawGradientAlpha(s,rect,ul,ur,dl,dr);
}
void AGSurface::drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2)
{
  CHECK_ZERO(s);
  AGDrawBorder(s,rect,W,c1,c2);
}
/*
void AGSurface::putPixel(int x,int y,const AGColor &c)
{
  CHECK_ZERO(s);
  }*/



 // blitting must be done manually as it doesn't work in GL mode
 // FIXME: do blitting in SDL-mode with SDL_BlitSurface!
void AGSurface::blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  //  CTRACE;
  int x,y,sx,sy,dx,dy;
  Uint32 c;
  Uint8 r,g,b,a;
  SDL_Surface *ss=const_cast<SDL_Surface*>(pSource.s);
  SDL_Surface *ds=s;

  for(y=0,sy=pSrc.y, dy=pDest.y;y<pSrc.h ; y++,sy++,dy++)
    for(x=0,sx=pSrc.x, dx=pDest.x;x<pSrc.w ; x++,sx++,dx++)
      {
	c=sge_GetPixel(ss,sx,sy);//x+sr.x,y+sr.y);

	SDL_GetRGBA(c,ss->format,&r,&g,&b,&a);
	c=SDL_MapRGBA(ds->format,r,g,b,a);

	sge_PutPixel(ds,x,y,c);

      }
}

///////////////////////////////////////////////////////////
// AGTexture
///////////////////////////////////////////////////////////


AGTexture::AGTexture(const AGTexture &t)
{
  s=t.s;
  w=t.w;
  h=t.h;
  mTW=t.mTW;
  mTH=t.mTH;
  mTextureID=t.mTextureID;
  mHasTexture=false;
  mTextureUsed=false;
  getTextureManager()->registerTexture(this);
}

AGTexture::AGTexture()
{
  s=0;
  init();
  getTextureManager()->registerTexture(this);
}

// only used by AG*Screen to create a new texture
AGTexture::AGTexture(SDL_Surface *ps,int W,int H):s(ps),w(W),h(H)
{
  //  CTRACE;
  //  cdebug(W<<"//"<<H);
  mTW=float(w)/float(s->w);
  mTH=float(h)/float(s->h);
  init();
  getTextureManager()->registerTexture(this);
}

AGTexture::~AGTexture()
{
  getTextureManager()->deregisterTexture(this);
}

void AGTexture::init()
{
  mTextureID=0;
  mHasTexture=false;
  mTextureUsed=false;
}


void AGTexture::testSave() const
{
  SDL_SaveBMP(s,"testSave.bmp");
}

float AGTexture::getTW() const
{
  return mTW;
}
float AGTexture::getTH() const
{
  return mTH;
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
  Uint32 c=sge_GetPixel(s,x,y);
  Uint8 r,g,b,a;
  SDL_GetRGBA(c,s->format,&r,&g,&b,&a);
  return AGColor(r,g,b,a);
}

bool AGTexture::hasTexture() const
{
  return mHasTexture;
}
void AGTexture::clearTexture()
{
  mHasTexture=false;
}
void AGTexture::clearTextureUsed()
{
  mTextureUsed=false;
}
void AGTexture::setTextureID(GLuint id)
{
  mHasTexture=true;
  mTextureID=id;
}
GLuint AGTexture::getTextureID()
{
  //  CTRACE;
  assert(mHasTexture);
  mTextureUsed=true;
  return mTextureID;
}

bool AGTexture::textureUsed() const
{
  //  CTRACE;
  return mTextureUsed;
}

SDL_Surface *AGTexture::surface()
{
  return s;
}







////////////////////////////////////////////////////////////////////////
// AGScreen
////////////////////////////////////////////////////////////////////////



void AGScreen::flip()
{
}

/*
AGTexture AGScreen::displayFormat(SDL_Surface *s)
{
  STUB;
  return AGTexture();
}
*/
AGTexture AGScreen::makeTexture(const AGSurface &s)
{
  STUB;
  return AGTexture();
}

void AGScreen::deleteTexture(AGTexture &t)
{
  STUB;
}




/** AGSurfaceManager */
AGSurfaceManager::AGSurfaceManager()
{
}
AGSurfaceManager::~AGSurfaceManager()
{
  CTRACE;
  std::set<SDL_Surface*>::iterator i=mRealSurfaces.begin();
  for(;i!=mRealSurfaces.end();i++)
    {
      SDL_FreeSurface(*i);
    }
}

AGSurface AGSurfaceManager::loadSurface(const std::string &pFilename)
{
  std::string file=loadFile(pFilename);
  
  SDL_Surface *s=IMG_Load_RW(SDL_RWFromMem(const_cast<char*>(file.c_str()),file.length()),false);
  if(!s)
   {
     std::cout<<"Error loading file:"<<pFilename<<std::endl;
     return AGSurface();
   }

  return AGSurface(s);

}

AGSurface AGSurfaceManager::fromSDL(SDL_Surface *s)
{
  assert(s);
  return AGSurface(s);
}

size_t mRegisteredSurfaces=0;

void AGSurfaceManager::registerSurface(AGSurface *pSurface)
{
  mSurfaces.insert(pSurface);
  if(pSurface->valid())
    {
      // new real surfaces
      size_t old=mRealSurfaces.size();
      mRealSurfaces.insert(pSurface->surface());
      if(old!=mRealSurfaces.size())
	mRegisteredSurfaces++;
    }

  if((mRegisteredSurfaces>10 && mAutoGC) || mRegisteredSurfaces>200)
    {
      cleanup(); // run garbage collection
      mRegisteredSurfaces=0;
    }
}
void AGSurfaceManager::deregisterSurface(AGSurface *pSurface)
{
  mSurfaces.erase(pSurface);
}

void AGSurfaceManager::enableGC()
{
  mAutoGC=true;
}
void AGSurfaceManager::disableGC()
{
  mAutoGC=false;
}


void AGSurfaceManager::cleanup()
{
  CTRACE;
  std::set<SDL_Surface*> used;
  std::set<AGSurface*>::iterator i=mSurfaces.begin();
  for(;i!=mSurfaces.end();i++)
    if((*i)->valid())
      used.insert((*i)->surface());

  std::set<SDL_Surface*>::iterator j;
  for(j=mRealSurfaces.begin();j!=mRealSurfaces.end();j++)
    {
      if(used.find(*j)==used.end())
	SDL_FreeSurface(*j);
    }
  mRealSurfaces=used;
}

AGSurfaceManager *mSurfaceManager=0;
AGSurfaceManager *getSurfaceManager()
{
  if(mSurfaceManager==0)
    {
      mSurfaceManager=new AGSurfaceManager();
      
      REGISTER_SINGLETON(mSurfaceManager);
    }
  return mSurfaceManager;
}



/** AGTextureManager */
AGTextureManager::AGTextureManager()
{
}
AGTextureManager::~AGTextureManager()
{
  CTRACE;
  std::set<SDL_Surface*>::iterator i=mSurfaces.begin();
  for(;i!=mSurfaces.end();i++)
    {
      SDL_FreeSurface(*i);
    }
}

/*bool mTrap=false;
void setTrap()
{
  mTrap=true;
  }*/

void AGTextureManager::registerTexture(AGTexture *pTexture)
{
  //  CTRACE;
  //  cdebug(pTexture);
#ifdef SLOW_TEXTUREMANAGER
  mTextures.insert(pTexture);
#else
  mTextures.push_back(pTexture);
#endif
  //  if(mTrap)
  //    throw int();
}
void AGTextureManager::deregisterTexture(AGTexture *pTexture)
{
  //  CTRACE;
  //  cdebug(pTexture);
#ifdef SLOW_TEXTUREMANAGER
  mTextures.erase(pTexture);
#else
  mDelTextures.push_back(pTexture);
#endif
}
void AGTextureManager::cleanup()
{
#ifdef SLOW_TEXTUREMANAGER
  std::set<SDL_Surface*> used;
  std::set<AGTexture*>::iterator i=mTextures.begin();
  for(;i!=mTextures.end();i++)
    used.insert((*i)->surface());

  std::set<SDL_Surface*>::iterator j=mSurfaces.begin();
  for(;j!=mSurfaces.end();j++)
    {
      if(used.find(*j)==used.end())
	SDL_FreeSurface(*j);
    }
  mSurfaces=used;
#else

  Uint32 t0=SDL_GetTicks();

  if(mDelTextures.size())
    {
      // FIXME: is this faster ????
      // clean up mTextures
      // assuming that mTextures.size>mDelTextures.size => first insert mTextures then delete
      std::set<AGTexture*> ts;
      
      std::list<AGTexture*>::iterator di;
      
      for(di=mTextures.begin();di!=mTextures.end();di++)
	ts.insert(*di);
      
      for(di=mDelTextures.begin();di!=mDelTextures.end();di++)
	ts.erase(*di);
      
      mTextures.clear();
      mDelTextures.clear();
  
      std::set<AGTexture*>::iterator si;
      for(si=ts.begin();si!=ts.end();si++)
	mTextures.push_back(*si);
    }
  
  Uint32 t1=SDL_GetTicks();
  //  cdebug("TIME4:"<<t1-t0);

  std::set<SDL_Surface*> used;
  std::list<AGTexture*>::iterator i=mTextures.begin();
  for(;i!=mTextures.end();i++)
    used.insert((*i)->surface());

  std::set<SDL_Surface*>::iterator j=mSurfaces.begin();
  for(;j!=mSurfaces.end();j++)
    {
      if(used.find(*j)==used.end())
	SDL_FreeSurface(*j);
    }
  mSurfaces=used;


  Uint32 t2=SDL_GetTicks();
  //  cdebug("TIME5:"<<t2-t1);

#endif
}

AGTexture AGTextureManager::makeTexture(const AGSurface &s)
{
  return getScreen().makeTexture(s);
}

void AGTextureManager::checkUnused()
{
#ifdef SLOW_TEXTUREMANAGER
  std::set<AGTexture*>::iterator i=mTextures.begin();
#else
  std::list<AGTexture*>::iterator i=mTextures.begin();
#endif
  for(;i!=mTextures.end();i++)
    {
      if((*i)->hasTexture() && !(*i)->textureUsed())
	{
	  getScreen().deleteTexture(**i);
	  CTRACE;
	}
    }
  
}


AGTextureManager *mTextureManager=0;
AGTextureManager *getTextureManager()
{
  if(mTextureManager==0)
    {
      mTextureManager=new AGTextureManager();
      
      REGISTER_SINGLETON(mTextureManager);
    }
  return mTextureManager;
}


