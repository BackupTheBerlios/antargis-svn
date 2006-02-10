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
#include "GLee.h"

#include <SDL_opengl.h>
#include "ag_glsurface.h"
#include "ag_color.h"
#include "ag_debug.h"
#include "ag_fontengine.h"
#include "ag_surfacemanager.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <assert.h>
#include <set>
#include <map>
#include <vector>
#include <sge.h>
#include "SDL_image.h"

#include "ag_rendercontext.h"
#include <SDL.h>

#include <ag_fs.h>

#include "ag_triangle.h"
#include "ag_texture.h"

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
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
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
  glViewport( 0, 0, w, h );
  glDisable(GL_LIGHTING);
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
  myFlip();
  getSurfaceManager()->cleanup(); // FIXME: maybe delete it - or do cleanup in 2 stages
  initDraw();

  initGUIView(w,h);
}

size_t next2pow(size_t i)
{
  size_t j=1;
  while(j<i)
    j<<=1;

  return j;
}

//////////////////////////////////////////////////////////////
// ensures right color-order and flips image vertically
//////////////////////////////////////////////////////////////
SDL_Surface *toGLTexture(SDL_Surface *image, bool p3d=false)
{
  assert(image);

  int bytes = 4;

  size_t nw,nh;

  if(p3d)
    {
      nw=next2pow(image->w);
      nh=next2pow(image->h);
    }
  else
    {
      nh=nw=next2pow(std::max(image->w,image->h));
    }


  SDL_Surface* openGLSurface = SDL_CreateRGBSurface(    SDL_SWSURFACE, nw,nh, bytes*8,
                                                        0xff, 0xff<<8, 0xff<<16, 0xff<<24 );

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
	      
	      if(p3d)
		sge_PutPixel( openGLSurface, i, (image->h-1)-j, cprime); // invert y
	      else
		sge_PutPixel( openGLSurface, i, j, cprime); // invert y
	    }
	}
    }
  

  glTestSurfaces.insert(openGLSurface);
 
  return openGLSurface;
}

GLuint assignTexture(SDL_Surface *pSurface,int w,int h)
{
  assertGL;
  SDL_Surface* surface = pSurface;

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
  assertGL;
  glGenTextures( 1, &id);
  assertGL;
  glBindTexture( GL_TEXTURE_2D,id);

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

#ifndef OLD

  glTexSubImage2D( GL_TEXTURE_2D,
		   0,w,h,texSurface->w-w,texSurface->h-h,format,GL_UNSIGNED_BYTE,
		   texSurface->pixels );

#endif
  assertGL;

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  assertGL;

  SDL_FreeSurface( texSurface );

  assertGL;

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  
  glBindTexture( GL_TEXTURE_2D,0);
  return id;
}

GLuint assignTexture3D(SDL_Surface *pSurface)
{
  assertGL;
  SDL_Surface* surface = pSurface;

  assert(glTestSurfaces.find(pSurface)!=glTestSurfaces.end());

  assert( surface->w < surface->h );

  // Used to invert the surface vertically. There must be a better way.
  SDL_Surface* texSurface = SDL_CreateRGBSurface(       SDL_SWSURFACE,
                                                        surface->w, surface->h,
                                                        surface->format->BitsPerPixel,
                                                        0xff, 0xff<<8, 0xff<<16, 0xff<<24 );

  for( int j=0; j<surface->h; ++j )
    memcpy(     (Uint8*) texSurface->pixels + texSurface->pitch * ( surface->h - 1 - j),
                (Uint8*) surface->pixels + surface->pitch * j,
                surface->pitch );

  GLuint id;
  assertGL;
  glGenTextures( 1, &id);
  assertGL;
  glBindTexture( GL_TEXTURE_3D,id);

  int format = ( texSurface->format->BytesPerPixel == 4 ) ? GL_RGBA : GL_RGB;

  glTexImage3D( GL_TEXTURE_3D,
                0,                                      // no mip mapping
                format,
                texSurface->w,
                texSurface->w,
                texSurface->h/texSurface->w,
                0,                                      // no border
                format,                 // format
                GL_UNSIGNED_BYTE,
                texSurface->pixels );

  assertGL;

  glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  assertGL;
  SDL_FreeSurface( texSurface );

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  
  glBindTexture( GL_TEXTURE_2D,0);
  return id;
}



void deleteTexture(GLuint id)
{
  glDeleteTextures(1,&id);
}

bool AGGLScreen::inScreen(const AGRect &r) const
{
  return AGRect(0,0,w,h).contains(r);
}

void AGGLScreen::blit(const AGTexture &pSource,const AGRect &pRect,const AGRect &pSrc)
{
  blit(pSource,pRect,pSrc,AGColor(0xFF,0xFF,0xFF,0xFF));
}

void AGGLScreen::blit(const AGTexture &pSource,const AGRect &pRect,const AGRect &pSrc,const AGColor &pColor)
{
  //  CTRACE;
  if(!inScreen(pRect))
    return;

  int w2=std::min((int)pRect.w(),pSource.width());
  int h2=std::min((int)pRect.h(),pSource.height());

  float x0=pRect.x();
  float y0=h-1-pRect.y();
  float x1=pRect.x()+w2;
  float y1=h-1-(pRect.y()+h2);

  AGRenderContext context;
  context.setTexture(const_cast<AGTexture&>(pSource).glTexture());
  if(pColor.a>0)
    context.setColor(pColor);
  context.begin();

  //  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;

  float tx0,ty0,tx1,ty1;
  if(pSrc.x()==0 && pSrc.y()==0 && pSrc.w()==pSource.width() && pSrc.h()==pSource.height())
    {
      tx0=0;
      ty0=1;
      tx1=pSource.getTW();
      ty1=1-pSource.getTH();
    }
    else
    {
      float tw=pSource.getTextureWidth();
      float th=pSource.getTextureHeight();
      tx0=float(pSrc.x())/tw;
      ty0=1-float(pSrc.y())/th;
      tx1=float(pSrc.x()+pSrc.w())/tw;
      ty1=1-float(pSrc.y()+pSrc.h())/th;
    }

  ty0=1.0f-ty0;
  ty1=1.0f-ty1;

  glBegin(GL_TRIANGLES);

  glTexCoord2f(tx0,ty1);
  glVertex2f(x0,y1);
  glTexCoord2f(tx1,ty0);
  glVertex2f(x1,y0);
  glTexCoord2f(tx0,ty0);
  glVertex2f(x0,y0);


  glTexCoord2f(tx0,ty1);
  glVertex2f(x0,y1);
  glTexCoord2f(tx1,ty1);
  glVertex2f(x1,y1);
  glTexCoord2f(tx1,ty0);
  glVertex2f(x1,y0);

  glEnd();
}


void AGGLScreen::fillRect(const AGRect &pRect,const AGColor &c)
{
  glDisable(GL_CULL_FACE);

  float x0=pRect.x();
  float y0=h-pRect.y();
  float x1=pRect.x()+pRect.w();
  float y1=h-pRect.y()-pRect.h();

  AGRenderContext context;
  context.setColor(c);
  context.begin();

  // turned
  glBegin(GL_TRIANGLES);
  glVertex2f(x1,y0);
  glVertex2f(x0,y1);
  glVertex2f(x0,y0);

  glVertex2f(x1,y0);
  glVertex2f(x1,y1);
  glVertex2f(x0,y1);
  glEnd();
  glEnable(GL_CULL_FACE);
}

AGRect AGGLScreen::getRect() const
{
  return AGRect(0,0,w,h);
}

void glColor(const AGColor &c)
{
  glColor4f(c.r/255.0,c.g/255.0,c.b/255.0,c.a/255.0);
}

void AGGLScreen::drawGradientAlpha(const AGRect& pRect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  float x0=pRect.x();
  float y0=(h-1)-pRect.y();
  float x1=pRect.x()+pRect.w();
  float y1=(h-1)-(pRect.y()+pRect.h());

  // turned
  AGRenderContext context;

#warning "maybe here an error occures - because GL_COLOR_MATERIAL is not activated!"
  context.begin();

  glBegin(GL_TRIANGLES);
  glColor(dl);
  glVertex2f(x0,y1);
  glColor(ur);
  glVertex2f(x1,y0);
  glColor(ul);
  glVertex2f(x0,y0);


  glColor(dl);
  glVertex2f(x0,y1);
  glColor(dr);
  glVertex2f(x1,y1);
  glColor(ur);
  glVertex2f(x1,y0);
  glEnd();
}

void AGGLScreen::drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2)
{
  glDisable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  float x0=rect.x();
  float y0=h-1-rect.y();
  float x1=rect.x()+rect.w();
  float y1=h-1-(rect.y()+rect.h());

  AGRenderContext context;
  context.setColor(c1);
  context.begin();

  // left and //top
  glColor(c1);
  glBegin(GL_TRIANGLES);
  // top
  glVertex2f(x0+W,y0-W);
  glVertex2f(x1,y0);
  glVertex2f(x0,y0);

  glVertex2f(x0+W,y0-W);
  glVertex2f(x1-W,y0-W);
  glVertex2f(x1,y0);

  // left
  glVertex2f(x0+W,y1+W);
  glVertex2f(x0+W,y0-W);
  glVertex2f(x0,y0);

  glVertex2f(x0,y1);
  glVertex2f(x0+W,y1+W);
  glVertex2f(x0,y0);
  glEnd();

  // right and //bottom
  context.setColor(c2);
  context.begin();
  //  glColor(c2);
  glBegin(GL_TRIANGLES);
  // bottom
  glVertex2f(x0+W,y1+W);
  glVertex2f(x1,y1);
  glVertex2f(x0,y1);

  glVertex2f(x0+W,y1+W);
  glVertex2f(x1-W,y1+W);
  glVertex2f(x1,y1);

  // right
  glVertex2f(x1-W,y1+W);
  glVertex2f(x1-W,y0-W);
  glVertex2f(x1,y0);

  glVertex2f(x1,y1);
  glVertex2f(x1-W,y1+W);
  glVertex2f(x1,y0);
  glEnd();

  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);


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

/*
AGTexture AGGLScreen::makeTexture(const AGSurface &s)
{
  return AGTexture(const_cast<AGSurface&>(s).surface(),s.width(),s.height());
  return AGTexture(toGLTexture(const_cast<AGSurface&>(s).surface()),s.width(),s.height());
}


AGTexture AGGLScreen::makeTexture3D(const AGSurface &s)
{
  return AGTexture(toGLTexture(const_cast<AGSurface&>(s).surface(),true),s.width(),s.height(),true);
}

void AGGLScreen::deleteTexture(AGTexture &t)
{
  CTRACE;
  if(t.hasTexture())
    {
      t.clearTexture();
    }
}
*/

void AGGLScreen::drawGradient(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  drawGradientAlpha(rect,ul,ur,dl,dr);
}

void AGGLScreen::drawLine(const AGPoint &p0,const AGPoint &p1,const AGColor &c)
{
  AGRenderContext context;
  context.setColor(c);
  context.begin();

  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex2f(p0[0],h-1-p0[1]);
  glVertex2f(p1[0],h-1-p1[1]);
  glEnd();
}

void AGGLScreen::blitTri(const AGTexture &pSource,const AGTriangle2 &pSrc,const AGTriangle2 &pDest)
{
  AGRenderContext c;
  c.setTexture(const_cast<AGTexture&>(pSource).glTexture());
  c.begin();

  glBegin(GL_TRIANGLES);

  glTexCoord2f(pSrc[2].getX()*pSource.getTW(),1-pSrc[2].getY()*pSource.getTH());
  glVertex2f(pDest[2].getX(), h-1-pDest[2].getY());

  glTexCoord2f(pSrc[1].getX()*pSource.getTW(),1-pSrc[1].getY()*pSource.getTH());
  glVertex2f(pDest[1].getX(), h-1-pDest[1].getY());

  glTexCoord2f(pSrc[0].getX()*pSource.getTW(),1-pSrc[0].getY()*pSource.getTH());
  glVertex2f(pDest[0].getX(), h-1-pDest[0].getY());
  
  glEnd();
}

size_t AGGLScreen::getWidth() const
{
  return w;
}
size_t AGGLScreen::getHeight() const
{
  return h;
}
void AGGLScreen::clip(const AGRect &r)
{
  AGRect x=AGRect(0,0,w,h);
  AGRect m=x.intersect(r);
  //  cdebug(r<<"   "<<m<<"  "<<x);


  //  cdebug(m.x0()<<"  "<<h-1-m.y0()<<"  "<<m.width()<<"  "<<m.height());

  glScissor(m.x0(),h-1-m.y1(),m.width(),m.height());
  //  glScissor(0,100,w,h-100);
  assertGL;
  glEnable(GL_SCISSOR_TEST);
  assertGL;
}
void AGGLScreen::unclip()
{
  glDisable(GL_SCISSOR_TEST);
  assertGL;
}

/*
void AGGLScreen::tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  AGRenderContext context;
  context.setTexture(const_cast<AGTexture&>(pSource).glTexture());
  context.begin();



#ifdef OLD
  float x0=pDest.x();
  float y0=h-1-(pDest.y());

  float tw,th;
  float sx0,sx1,sy0,sy1;
  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;

  sx0=float(pSrc.x())/surface->w;
  sy0=1-float(pSrc.y())/surface->h;

  tw=pSrc.w()/surface->w;
  th=pSrc.h()/surface->h;

  glBegin(GL_TRIANGLES);

  float px0,py0,px1,py1;
  float px,py;

  for(px=0,px0=x0;px<pDest.w();px+=pSrc.w(), px0+=pSrc.w())
    for(py=0,py0=y0;py<pDest.h();py+=pSrc.h(), py0-=pSrc.h())
      {
	px1=px0+pSrc.w();
	py1=py0-pSrc.h();

        // turned
        sx1=sx0+tw;
        sy1=sy0-th;

        glTexCoord2f(sx0,sy1);
        glVertex2f(px0,py1);

        glTexCoord2f(sx1,sy0);
        glVertex2f(px1,py0);

        glTexCoord2f(sx0,sy0);
        glVertex2f(px0,py0);


        glTexCoord2f(sx0,sy1);
        glVertex2f(px0,py1);

        glTexCoord2f(sx1,sy1);
        glVertex2f(px1,py1);

        glTexCoord2f(sx1,sy0);
        glVertex2f(px1,py0);



      }


  glEnd();
#else
  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;

  glDisable(GL_CULL_FACE);
  cdebug(pSrc);

  AGRect d(100,100,100,100);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, surface->pitch/surface->format->BytesPerPixel);


  glTexSubImage2D(GL_TEXTURE_2D,0,pSrc.x(),pSrc.y(),pSrc.w(),pSrc.h(),GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);
  //  glTexSubImage2D(GL_TEXTURE_2D,0,1,0,surface->w-1,surface->h,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);

  float tx=pDest.width()/pSrc.width();
  float ty=pDest.height()/pSrc.height();

  tx=ty=1;

  glBegin(GL_QUADS);

  glTexCoord2f(0,0);
  glVertex2f(d.x0(),h-1-d.y0());

  glTexCoord2f(0,ty);
  glVertex2f(d.x0(),h-1-d.y1());

  glTexCoord2f(tx,ty);
  glVertex2f(d.x1(),h-1-d.y1());

  glTexCoord2f(tx,0);
  glVertex2f(d.x1(),h-1-d.y0());

  glEnd();

  //glTexSubImage2D(GL_TEXTURE_2D,0,0,0,surface->w,surface->h,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);
#endif
  assertGL;
}
*/

bool opengl()
{
  return dynamic_cast<AGGLScreen*>(&getScreen());
}
