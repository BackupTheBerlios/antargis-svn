/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_glsurface.cc
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

#include "ag_glsurface.h"
#include "ag_color.h"
#include "ag_debug.h"
#include "ag_fontengine.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <assert.h>
#include <set>
#include <map>
#include <vector>
#include <sge.h>
#include "SDL_image.h"

#include <SDL.h>

#include <ag_fs.h>

#include "ag_vector.h"
#include "ag_triangle.h"


std::set<SDL_Surface *> glTestSurfaces;


void initDraw()
{
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glDisable(GL_DEPTH_TEST); // enable depth test
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear screen and depth buffer
  glLoadIdentity();
  glDepthMask(false);
  //  glTranslatef(0.375, 0.375, 0.0);

}

void initGUIView(int w,int h)
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  GLfloat ratio;

  ratio = ( float )w / ( float )h;

  //  gluPerspective( 45.0f, ratio, 1.0f, 100.0f );

  gluOrtho2D(0,w,0,h);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );

}

void myFlip()
{
  //  TRACE;
  SDL_GL_SwapBuffers();
//  SDL_Delay(10);
}

AGGLScreen::AGGLScreen(int W,int H):
  w(W),h(H)
{
  // init GL
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f,0.0f,0.0f,0.0f); // clear bgcolor
  glClearDepth(1.0f);      // clear depth buffer
  glEnable(GL_DEPTH_TEST); // enable depth test
  glDepthFunc(GL_LEQUAL); // set type depth test
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // GL_NICEST // best perspective correction
  glEnable(GL_BLEND);

  glViewport( 0, 0, w, h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  GLfloat ratio;

  ratio = ( float )w / ( float )h;

  //  gluPerspective( 45.0f, ratio, 1.0f, 100.0f );

  gluOrtho2D(0,w,0,h);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  //  glTranslatef(0.375, 0.375, 0.0);

}




AGGLScreen::~AGGLScreen()
{ 
}

void AGGLScreen::begin()
{
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  // glClearColor(0.0f,0.0f,0.0f,0.0f); // clear bgcolor
  //  glClearDepth(1.0f);      // clear depth buffer
  glEnable(GL_DEPTH_TEST); // enable depth test
  glDepthFunc(GL_LEQUAL); // set type depth test
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // GL_NICEST // best perspective correction
  glEnable(GL_BLEND);

  glViewport( 0, 0, w, h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  GLfloat ratio;

  ratio = ( float )w / ( float )h;

  //  gluPerspective( 45.0f, ratio, 1.0f, 100.0f );

  gluOrtho2D(0,w,0,h);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );


  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glDisable(GL_DEPTH_TEST); // enable depth test

  glDepthMask(false);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//NEAREST);//LINEAR);
  glEnable(GL_COLOR_MATERIAL);
}

void AGGLScreen::flip()
{
  //  paintTerrain();
  //  TRACE;
  myFlip();
  getTextureManager()->cleanup(); // FIXME: maybe delete it - or do cleanup in 2 stages
  getTextureManager()->checkUnused();
  //  checkUnusedTextures();
  initDraw();

  //  mUsedTextures.clear();

  initGUIView(w,h);
}

size_t next2pow(size_t i)
{
  size_t j=1;
  while(j<i)
    j<<=1;

  return j;
}

SDL_Surface *toGLTexture(SDL_Surface *image)
{
  assert(image);

  int bytes = 4;
  //  if ( image->format->BitsPerPixel == 32 )
  //    bytes = 4;

  size_t nw=next2pow(std::max(image->w,image->h));

  SDL_Surface* openGLSurface = SDL_CreateRGBSurface(    SDL_SWSURFACE, nw,nw, bytes*8,
                                                        0xff, 0xff<<8, 0xff<<16, 0xff<<24 );

  //  cdebug((int)(image->format->BitsPerPixel));
  if (false)//image->format->BitsPerPixel != 32 )
    {
      //      TRACE;
      SDL_BlitSurface( image, 0, openGLSurface, 0 );
      SDL_Flip(openGLSurface);
    }
  else
    {
      //      TRACE;
      Uint8 r, g, b, a;
      Uint32 ckey=image->format->colorkey;

      // Grr! Screws up alpha channel. Fix the colors.
      for( int j=0; j<image->h; ++j )
        {
          for( int i=0; i<image->w; ++i )
            {
              Uint32 c = sge_GetPixel( image, i, j );
	      if(c!=image->format->colorkey)
		{
		  SDL_GetRGBA( c, image->format, &r, &g, &b, &a );
		  if(c==ckey)
		    a=0;//r=255,b=0;
		  //		  a=255;
		  Uint32 cprime = SDL_MapRGBA( openGLSurface->format, r, g, b, a );
		 
		  sge_PutPixel( openGLSurface, i, j, cprime);
		  //sge_PutPixelAlpha( openGLSurface, i, j, cprime,a);



		  //		  sge_PutPixelAlpha( openGLSurface, i, j, cprime,a );
		}
            }
        }
    }


  glTestSurfaces.insert(openGLSurface);
  //  SDL_FreeSurface( image );
  return openGLSurface;
}
/*
AGSurface AGGLScreen::loadSurface(const std::string &pFilename)
{
  std::string file=loadFile(pFilename);
  

  //  CTRACE;
  //SDL_Surface *s=IMG_Load(pFilename.c_str());
  SDL_Surface *s=IMG_Load_RW(SDL_RWFromMem(const_cast<char*>(file.c_str()),file.length()),false);
  if(!s)
   {
     std::cout<<"Error loading file:"<<pFilename<<std::endl;
     return AGSurface();
   }
  int w,h;
  w=s->w;
  h=s->h;
  //  SDL_SaveBMP(s,"load1.bmp");
  //  SDL_Surface *n=toGLTexture(s);
  //  SDL_FreeSurface(s);
  //  SDL_SaveBMP(n,"load2.bmp");
  //  cdebug("load:"<<n);
  return AGSurface(s,w,h);
}
*/

GLuint assignTexture(SDL_Surface *pSurface)
{
  //  TRACE;
  SDL_Surface* surface = pSurface;//toGLTexture(pSurface);
  //  assert(surface);

  assert(glTestSurfaces.find(pSurface)!=glTestSurfaces.end());

  int size = surface->w;

  assert( surface->w == surface->h );

  // Used to invert the surface. There must be a better way.
  SDL_Surface* texSurface = SDL_CreateRGBSurface(       SDL_SWSURFACE,
                                                        size, size,
                                                        surface->format->BitsPerPixel,
                                                        0xff, 0xff<<8, 0xff<<16, 0xff<<24 );

  for( int j=0; j<size; ++j )
    memcpy(     (Uint8*) texSurface->pixels + texSurface->pitch * ( size - 1 - j),
                (Uint8*) surface->pixels + surface->pitch * j,
                size * texSurface->format->BytesPerPixel );

  GLuint id;
  glGenTextures( 1, &id);
  //  cdebug("newid:"<<id);
  //  textureIDs[textureName]=id;
  glBindTexture( GL_TEXTURE_2D,id);// textureIDs[textureName] );

  //  cout<<"textureID:"<<textureIDs[textureName]<<endl;

  int format = ( texSurface->format->BytesPerPixel == 4 ) ? GL_RGBA : GL_RGB;

  glTexImage2D( GL_TEXTURE_2D,
                0,                                      // no mip mapping
                format,
                texSurface->w,
                texSurface->h,
                0,                                      // no border
                format,                 // format
                GL_UNSIGNED_BYTE,
                texSurface->pixels );

  assertGL;
  //  assert( glGetError() == GL_NO_ERROR );

  SDL_FreeSurface( texSurface );
  //  SDL_FreeSurface( surface );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//NEAREST);//LINEAR);

  assertGL;
  //  assert( glGetError() == GL_NO_ERROR );

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  
  glBindTexture( GL_TEXTURE_2D,0);
  //  cdebug(id);
  return id;
}

void deleteTexture(GLuint id)
{
  glDeleteTextures(1,&id);
}



#ifdef NEW_TEXTURES
GLuint AGGLScreen::getID(AGTexture *s)
{
  //  CTRACE;
  //  cdebug("texture:"<<s);
  //  cdebug(s->hasTexture());
  if(s->hasTexture())
    return s->getTextureID();

  //  cdebug("assign..");
  GLuint id=assignTexture(s->surface());
  //  cdebug("ID:"<<id);
  //  cdebug("rect:"<<s->getRect());
  s->setTextureID(id);
  //  mTextures.push_back(s);
  assert(s->hasTexture());
  // cdebug(s->hasTexture());

 // SDL_SaveBMP(s->surface(),"texture.bmp");
  return s->getTextureID();// set used
}
#else
TextureID AGGLScreen::getID(SDL_Surface *s)
{
  //  if(mGLSurfaces[s]==0)
  //    mGLSurfaces[s]=toGLTexture(s);
  //  if(mTexturesInv.size())
    //    return mTexturesInv.begin()->second;

  SDL_Surface *p=s;//mGLSurfaces[s];
  GLuint id;

  if(mTexturesInv.find(p)==mTexturesInv.end())
    {
      mTexturesInv[p]=assignTexture(p);
      mSurfaceRect[s]=AGRect(0,0,s->w,s->h);
    }
  id=mTexturesInv[p];
  mTextures[mTexturesInv[p]]=p;

  mUsedTextures[id]=true;

  return id;
}
#endif
/*
AGRect AGGLScreen::getRect(SDL_Surface *s)
{
  return mSurfaceRect[s];
  }*/

bool AGGLScreen::inScreen(const AGRect &r) const
{
  return !(r.x>w || r.y>h || (r.x+r.w-1<0) || (r.y+r.h-1<0));
}

bool AGGLScreen::inScreen(const AGRect2 &r) const
{
  return !(r.x()>w || r.y()>h || (r.x()+r.w()-1<0) || (r.y()+r.h()-1<0));
}



void AGGLScreen::blit(const AGTexture &pSource,const AGRect &pRect)
{
  //  CTRACE;
  if(!inScreen(pRect))
    return;


  //  TRACE;
  int w2=std::min((int)pRect.w,pSource.width());
  int h2=std::min((int)pRect.h,pSource.height());

  float x0=pRect.x;
  float y0=h-1-pRect.y;
  float x1=pRect.x+w2;
  float y1=h-1-(pRect.y+h2);

  //  cdebug(x0<<";"<<y0<<";"<<x1<<";"<<y1);
  //  cdebug(pSource.width()<<";"<<pSource.height());
  
  //  cdebug(tw<<";"<<th);
  
  
  //  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;
  
  //cdebug(surface);
  //  SDL_SaveBMP(surface,"test.bmp");

#ifdef NEW_TEXTURES
  GLuint id=getID(const_cast<AGTexture*>(&pSource));
#else
  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;
  TextureID id=getID(surface);
#endif

  glBindTexture( GL_TEXTURE_2D,id);
  assertGL;

  assert( glGetError() == GL_NO_ERROR );

  //  AGRect sRect=getRect(surface);

  //  float tw=float(pSource.width())/surface->w;
  //  float th=float(pSource.height())/surface->h;

  float tw=pSource.getTW();
  float th=pSource.getTH();
  //  cdebug(tw<<"//"<<tw2);
  //  cdebug(th<<"//"<<th2);
  //float tw=pSource.getTW();
  //  float th=pSource.getTH();

  //  cdebug(tw<<";"<<th);

  glColor4f(1,1,1,1);
  glBegin(GL_TRIANGLES);

  glTexCoord2f(0,1);
  glVertex2f(x0,y0);
  glTexCoord2f(tw,1);
  glVertex2f(x1,y0);
  glTexCoord2f(0,1-th);
  glVertex2f(x0,y1);

  glTexCoord2f(tw,1);
  glVertex2f(x1,y0);
  glTexCoord2f(tw,1-th);
  glVertex2f(x1,y1);
  glTexCoord2f(0,1-th);
  glVertex2f(x0,y1);

  glEnd();
  glBindTexture( GL_TEXTURE_2D,0);
}

void AGGLScreen::blit(const AGTexture &pSource,const AGRect &pRect,const AGRect &pSrc)
{
  blit(pSource,pRect,pSrc,AGColor(0xFF,0xFF,0xFF,0xFF));
}

void AGGLScreen::blit(const AGTexture &pSource,const AGRect2 &pRect,const AGRect2 &pSrc)
{
  if(!inScreen(pRect))
    return;

  float w2=std::min(pRect.w(),(float)pSource.width());
  float h2=std::min(pRect.h(),(float)pSource.height());

  float x0=pRect.x();
  float y0=h-1-pRect.y();
  float x1=pRect.x()+w2;
  float y1=h-1-(pRect.y()+h2);

#ifdef NEW_TEXTURES
  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;
  GLuint id=getID(const_cast<AGTexture*>(&pSource));
#else
  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;
  TextureID id=getID(surface);
#endif
  //  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;
  
  //  TextureID id=getID(surface);

  glBindTexture( GL_TEXTURE_2D,id);
  assert( glGetError() == GL_NO_ERROR );

  //  AGRect sRect=getRect(surface);

  float tx0,ty0,tx1,ty1;
  if(pSrc.x()==0 && pSrc.y()==0 && pSrc.w()==pSource.width() && pSrc.h()==pSource.height())
    {
      tx0=ty0=0;
      tx1=pSource.getTW();
      ty1=pSource.getTH();
    }
    else
    {
      tx0=float(pSrc.x())/surface->w;
      ty0=float(pSrc.y())/surface->w;
      tx1=float(pSrc.x()+pSrc.w())/surface->w;
      ty1=float(pSrc.y()+pSrc.h())/surface->w;
    }

  ty0=1.0f-ty0;
  ty1=1.0f-ty1;

  glColor4f(1,1,1,1);
  /*  glColor4f(float(pColor.r)/0xFF,
	    float(pColor.g)/0xFF,
	    float(pColor.b)/0xFF,
	    float(pColor.a)/0xFF);*/
  glBegin(GL_TRIANGLES);

  glTexCoord2f(tx0,ty0);
  glVertex2f(x0,y0);
  glTexCoord2f(tx1,ty0);
  glVertex2f(x1,y0);
  glTexCoord2f(tx0,ty1);
  glVertex2f(x0,y1);

  glTexCoord2f(tx1,ty0);
  glVertex2f(x1,y0);
  glTexCoord2f(tx1,ty1);
  glVertex2f(x1,y1);
  glTexCoord2f(tx0,ty1);
  glVertex2f(x0,y1);

  glEnd();
  glBindTexture( GL_TEXTURE_2D,0);
}


void AGGLScreen::blit(const AGTexture &pSource,const AGRect &pRect,const AGRect &pSrc,const AGColor &pColor)
{
  if(!inScreen(pRect))
    return;

  int w2=std::min((int)pRect.w,pSource.width());
  int h2=std::min((int)pRect.h,pSource.height());

  float x0=pRect.x;
  float y0=h-1-pRect.y;
  float x1=pRect.x+w2;
  float y1=h-1-(pRect.y+h2);

#ifdef NEW_TEXTURES
  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;
  GLuint id=getID(const_cast<AGTexture*>(&pSource));
#else
  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;
  TextureID id=getID(surface);
#endif
  //  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;
  
  //  TextureID id=getID(surface);

  glBindTexture( GL_TEXTURE_2D,id);
  assert( glGetError() == GL_NO_ERROR );

  //  AGRect sRect=getRect(surface);

  float tx0,ty0,tx1,ty1;
  if(pSrc.x==0 && pSrc.y==0 && pSrc.w==pSource.width() && pSrc.h==pSource.height())
    {
      tx0=ty0=0;
      tx1=pSource.getTW();
      ty1=pSource.getTH();
    }
    else
    {
      tx0=float(pSrc.x)/surface->w;
      ty0=float(pSrc.y)/surface->w;
      tx1=float(pSrc.x+pSrc.w)/surface->w;
      ty1=float(pSrc.y+pSrc.h)/surface->w;
    }

  ty0=1.0f-ty0;
  ty1=1.0f-ty1;

  glColor4f(float(pColor.r)/0xFF,
	    float(pColor.g)/0xFF,
	    float(pColor.b)/0xFF,
	    float(pColor.a)/0xFF);
  glBegin(GL_TRIANGLES);

  glTexCoord2f(tx0,ty0);
  glVertex2f(x0,y0);
  glTexCoord2f(tx1,ty0);
  glVertex2f(x1,y0);
  glTexCoord2f(tx0,ty1);
  glVertex2f(x0,y1);

  glTexCoord2f(tx1,ty0);
  glVertex2f(x1,y0);
  glTexCoord2f(tx1,ty1);
  glVertex2f(x1,y1);
  glTexCoord2f(tx0,ty1);
  glVertex2f(x0,y1);

  glEnd();
  glBindTexture( GL_TEXTURE_2D,0);
}



void AGGLScreen::tile(const AGTexture &pSource)
{
  CTRACE;
#ifdef NEW_TEXTURES
  TextureID id=getID(const_cast<AGTexture*>(&pSource));//urface());
#else
  TextureID id=getID(const_cast<AGTexture&>(pSource).s);//urface());
#endif
  float x0=0;
  float y0=h-1;
  float x1=w-1;
  float y1=0;

  float w2=x1-x0;
  float h2=y1-y0;

  float mw=float(w2)/pSource.width();
  float mh=float(h2)/pSource.height();

  glBindTexture( GL_TEXTURE_2D,id);
  glColor4f(1,1,1,1);
  assert( glGetError() == GL_NO_ERROR );

  glBegin(GL_TRIANGLES);
  glTexCoord2f(0,mh);
  glVertex2f(x0,y0);

  glTexCoord2f(mw,mh);
  glVertex2f(x1,y0);

  glTexCoord2f(0,0);
  glVertex2f(x0,y1);
  
  glTexCoord2f(mw,mh);
  glVertex2f(x1,y0);

  glTexCoord2f(mw,0);
  glVertex2f(x1,y1);

  glTexCoord2f(0,0);
  glVertex2f(x0,y1);

  glEnd();
  glBindTexture( GL_TEXTURE_2D,0);
}
void AGGLScreen::tile(const AGTexture &pSource,const AGRect &pDest)
{
  //  CTRACE;
  if(!inScreen(pDest))
    return;
  tile(pSource,pDest,AGRect(0,0,pSource.width(),pSource.height()));
  return;
#ifdef NEW_TEXTURES
  TextureID id=getID(const_cast<AGTexture*>(&pSource));//urface());
#else
  TextureID id=getID(const_cast<AGTexture&>(pSource).s);//urface());
#endif

  float x0=pDest.x;
  float y0=h-1-(pDest.y);
  float x1=pDest.x+pDest.w;
  float y1=h-1-(pDest.y+pDest.h);

  float w2=x1-x0;
  float h2=y0-y1;

  float mw=float(w2)/pSource.width();
  float mh=float(h2)/pSource.height();

  glBindTexture( GL_TEXTURE_2D,id);
  glColor4f(1,1,1,1);
  assert( glGetError() == GL_NO_ERROR );

  glBegin(GL_TRIANGLES);
  glTexCoord2f(0,mh);
  glVertex2f(x0,y0);

  glTexCoord2f(mw,mh);
  glVertex2f(x1,y0);

  glTexCoord2f(0,0);
  glVertex2f(x0,y1);
  
  glTexCoord2f(mw,mh);
  glVertex2f(x1,y0);

  glTexCoord2f(mw,0);
  glVertex2f(x1,y1);

  glTexCoord2f(0,0);
  glVertex2f(x0,y1);

  glEnd();
  glBindTexture( GL_TEXTURE_2D,0);
}
void AGGLScreen::tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  if(!inScreen(pDest))
    return;

#ifdef NEW_TEXTURES
  TextureID id=getID(const_cast<AGTexture*>(&pSource));//urface());
#else
  TextureID id=getID(const_cast<AGTexture&>(pSource).s);//urface());
#endif

  float x0=pDest.x;
  float y0=h-1-(pDest.y);

  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;

  float tw,th;
  float sx0,sx1,sy0,sy1;

  sx0=float(pSrc.x)/surface->w;
  sy0=1-float(pSrc.y)/surface->h;

  glBindTexture( GL_TEXTURE_2D,id);
  glColor4f(1,1,1,1);
  glBegin(GL_TRIANGLES);
  for(int px=0;px<pDest.w;px+=pSrc.w)
    for(int py=0;py<pDest.h;py+=pSrc.h)
      {
	float px0=x0+px;
	float py0=y0-py;
	float px1=px0+std::min((int)pSrc.w,pDest.w-px);
	float py1=py0-std::min((int)pSrc.h,pDest.h-py);
	//	float px1=x1+std::min(px;
	//	float py1=y1-py;

	tw=float(std::min((int)pSrc.w,pDest.w-px))/surface->w;
	th=float(std::min((int)pSrc.h,pDest.h-py))/surface->h;

	sx1=sx0+tw;
	sy1=sy0-th;

	glTexCoord2f(sx0,sy0);
	glVertex2f(px0,py0);

	glTexCoord2f(sx1,sy0);
	glVertex2f(px1,py0);

	glTexCoord2f(sx0,sy1);
	glVertex2f(px0,py1);

	
	glTexCoord2f(sx1,sy0);
	glVertex2f(px1,py0);

	glTexCoord2f(sx1,sy1);
	glVertex2f(px1,py1);

	glTexCoord2f(sx0,sy1);
	glVertex2f(px0,py1);
      }
  glEnd();
	
  glBindTexture( GL_TEXTURE_2D,0);

}

void AGGLScreen::drawRect(const AGRect &pRect,const AGColor &c)
{
  //  return;
  float x0=pRect.x;
  float y0=h-pRect.y;
  float x1=pRect.x+pRect.w;
  float y1=h-pRect.y-pRect.h;

  glBindTexture(GL_TEXTURE_2D,0);
  glColor4f(c.r/255.0,c.g/255.0,c.b/255.0,c.a/255.0);


  glBegin(GL_TRIANGLES);
  glVertex2f(x0,y0);
  glVertex2f(x0,y1);
  glVertex2f(x1,y0);
  glVertex2f(x0,y1);
  glVertex2f(x1,y1);
  glVertex2f(x1,y0);
  glEnd();
}

AGRect AGGLScreen::getRect() const
{
  return AGRect(0,0,w,h);
}

void glColor(const AGColor &c)
{
  //  cdebug((int)c.r<<"/"<<(int)c.g<<"/"<<(int)c.b<<"/"<<(int)c.a);
  glColor4f(c.r/255.0,c.g/255.0,c.b/255.0,c.a/255.0);
}

void AGGLScreen::drawGradientAlpha(const AGRect& pRect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  //    TRACE;
  float x0=pRect.x;
  float y0=(h-1)-pRect.y;
  float x1=pRect.x+pRect.w;
  float y1=(h-1)-(pRect.y+pRect.h);

  AGColor ul2=AGColor(255,0,0);

  glBegin(GL_TRIANGLES);
  glColor(ul);
  glVertex2f(x0,y0);
  glColor(ur);
  glVertex2f(x1,y0);
  glColor(dl);
  glVertex2f(x0,y1);
  glColor(ur);
  glVertex2f(x1,y0);
  glColor(dr);
  glVertex2f(x1,y1);
  glColor(dl);
  glVertex2f(x0,y1);
  glEnd();
}

void AGGLScreen::drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2)
{
  //  TRACE;

  //  cdebug(rect);
  //  cdebug(c1<<"//"<<c2);
  float x0=rect.x;
  float y0=h-1-rect.y;
  float x1=rect.x+rect.w;
  float y1=h-1-(rect.y+rect.h);

  //  W=3;
  glBindTexture(GL_TEXTURE_2D,0);

  // left and //top
  glColor(c1);
  glBegin(GL_TRIANGLES);
  // top
  glVertex2f(x0,y0);
  glVertex2f(x1,y0);
  glVertex2f(x0+W,y0-W);

  glVertex2f(x1,y0);
  glVertex2f(x1-W,y0-W);
  glVertex2f(x0+W,y0-W);

  // left
  glVertex2f(x0,y0);
  glVertex2f(x0+W,y0-W);
  glVertex2f(x0+W,y1+W);

  glVertex2f(x0,y0);
  glVertex2f(x0+W,y1+W);
  glVertex2f(x0,y1);
  glEnd();

  // right and //bottom
  glColor(c2);
  glBegin(GL_TRIANGLES);
  // bottom
  glVertex2f(x0,y1);
  glVertex2f(x1,y1);
  glVertex2f(x0+W,y1+W);

  glVertex2f(x1,y1);
  glVertex2f(x1-W,y1+W);
  glVertex2f(x0+W,y1+W);

  // right
  glVertex2f(x1,y0);
  glVertex2f(x1-W,y0-W);
  glVertex2f(x1-W,y1+W);

  glVertex2f(x1,y0);
  glVertex2f(x1-W,y1+W);
  glVertex2f(x1,y1);
  glEnd();



}

void AGGLScreen::putPixel(int x,int y,const AGColor &pc)
{
  float X=x;
  float Y=h-1-y;

  Uint32 c;
  c=(pc.r<<24)|(pc.g<<16)|(pc.b<<8)|pc.a;

  glRasterPos2i((int)X,(int)Y);
  glDrawPixels(1,1,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,&c);
}

AGTexture AGGLScreen::makeTexture(const AGSurface &s)
{
  //  CTRACE;
  return AGTexture(toGLTexture(const_cast<AGSurface&>(s).surface()),s.width(),s.height());
}

void AGGLScreen::deleteTexture(AGTexture &t)
{
  if(t.hasTexture())
    {
      ::deleteTexture(t.getTextureID());
      t.clearTexture();
    }
}


void AGGLScreen::drawGradient(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  drawGradientAlpha(rect,ul,ur,dl,dr);
}
void AGGLScreen::renderText (const AGRect &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn)
{
  if(!AGFontEngine::renderText(this,pClipRect,BaseLineX,BaseLineY,pText,ParamIn))
    cdebug("SOME ERROR");
}

void AGGLScreen::drawLine(const AGPoint &p0,const AGPoint &p1,const AGColor &c)
{
  glBindTexture(GL_TEXTURE_2D,0);
  glColor(c);
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex2f(p0.x,h-1-p0.y);
  glVertex2f(p1.x,h-1-p1.y);
  glEnd();
}

void AGGLScreen::blitTri(const AGTexture &pSource,const AGTriangle2 &pSrc,const AGTriangle2 &pDest)
{
  GLuint id=getID(const_cast<AGTexture*>(&pSource));
  
  glBindTexture( GL_TEXTURE_2D,id);
  assert( glGetError() == GL_NO_ERROR );

  glColor4f(1,1,1,1);

  glBegin(GL_TRIANGLES);


  //  glTexCoord2fv(pSrc[0]);
  glTexCoord2f(pSrc[0].getX()*pSource.getTW(),1-pSrc[0].getY()*pSource.getTH());
  glVertex2f(pDest[0].getX(), h-1-pDest[0].getY());

  //  glTexCoord2fv(pSrc[1]);
  glTexCoord2f(pSrc[1].getX()*pSource.getTW(),1-pSrc[1].getY()*pSource.getTH());
  glVertex2f(pDest[1].getX(), h-1-pDest[1].getY());

  //  glTexCoord2fv(pSrc[2]);
  glTexCoord2f(pSrc[2].getX()*pSource.getTW(),1-pSrc[2].getY()*pSource.getTH());
  glVertex2f(pDest[2].getX(), h-1-pDest[2].getY());
  
  glEnd();
}
