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
#include "ag_sgeexport.h"
#include "SDL_image.h"

#include <ag_rendercontext.h>
#include <SDL.h>

#include <ag_fs.h>

#include <ag_geometry.h>
#include <ag_texture.h>
#include <ag_profiler.h>
#include <ag_glpainter.h>

std::set<SDL_Surface *> glTestSurfaces;

#define SIMPLE


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

  gluOrtho2D(0,w,0,h);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );

}

void myFlip()
{
  SDL_GL_SwapBuffers();
}

AGGLScreen::AGGLScreen(int W,int H,int VW,int VH):
  rw(W),rh(H),
  w(VW),h(VH)
{
  if(w<rw)
    w=rw;
  if(h<rh)
    h=rh;

  cdebug("w:"<<w<<" h:"<<h);

  // init GL
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f,0.0f,0.0f,0.0f); // clear bgcolor
  glClearDepth(1.0f);      // clear depth buffer
  glEnable(GL_DEPTH_TEST); // enable depth test
  glDepthFunc(GL_LEQUAL); // set type depth test
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // GL_NICEST // best perspective correction
  glEnable(GL_BLEND);

  glViewport( 0, 0, rw, rh );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  GLfloat ratio;

  ratio = ( float )w / ( float )h;

  //  gluPerspective( 45.0f, ratio, 1.0f, 100.0f );

  gluOrtho2D(0,w,0,h);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
}




AGGLScreen::~AGGLScreen()
{ 
}

AGSurface AGGLScreen::screenshot()
{
  AGSurface s(getWidth(),getHeight());

  SDL_Surface *surface=s.surface()->surface;

  unsigned char *buffer=new unsigned char[getWidth()*getHeight()*4];
  glReadBuffer(GL_FRONT);

  cdebug(s.surface());
  cdebug(s.surface()->surface);

  glPixelStorei(GL_PACK_ALIGNMENT, 2);
  assertGL;
  glPixelStorei(GL_PACK_ROW_LENGTH, getWidth());
  //                surface->pitch / surface->format->BytesPerPixel);
  assertGL;


  {
    TRACE;
    glReadPixels(0,0,getWidth(),getHeight(),GL_RGBA,GL_UNSIGNED_BYTE,buffer);//s.surface()->surface);
    glReadBuffer(GL_BACK);
  }

  {
    // copy
    TRACE;
    int h=getHeight()-1;
    for(int x=0;x<getWidth();x++)
      for(int y=0;y<getHeight();y++)
	{
	  unsigned char*p=buffer+((x+(h-y)*getWidth())*4);
	  AGColor c(p[0],p[1],p[2],p[3]);
	  s.putPixel(x,y,c);
	}
  }
  

  delete [] buffer;
  return s;
}


void AGGLScreen::begin()
{
  glViewport( 0, 0, rw, rh );
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);

  glEnable(GL_DEPTH_TEST); // enable depth test
  glDepthFunc(GL_LEQUAL); // set type depth test
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // GL_NICEST // best perspective correction
  glEnable(GL_BLEND);

  glViewport( 0, 0, rw, rh );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  GLfloat ratio;

  ratio = ( float )w / ( float )h;

  gluOrtho2D(0,w,0,h);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glDisable(GL_DEPTH_TEST); // enable depth test

  glDepthMask(false);
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


bool AGGLScreen::inScreen(const AGRect2 &r) const
{
  return AGRect2(0,0,w,h).contains(r);
}

void AGGLScreen::blit(const AGTexture &pSource,const AGRect2 &pRect,const AGRect2 &pSrc)
{
  blit(pSource,pRect,pSrc,AGColor(0xFF,0xFF,0xFF,0xFF));
}

void AGGLScreen::blit(const AGTexture &pSource,const AGRect2 &pRect,const AGRect2 &pSrc,const AGColor &pColor)
{
  AGGLPainter::blit(pSource,pRect,pSrc,pColor);
}


void AGGLScreen::blit(const AGTexture &pSource,const std::vector<std::pair<AGRect2,AGRect2> > &pSrcDestRects,const AGColor &pColor)
{
  AGGLPainter::blit(pSource,pSrcDestRects,pColor);
}


void AGGLScreen::tile(const AGTexture &pSource,const AGRect2 &pTarget,const AGColor &pColor)
{
  AGGLPainter::tile(pSource,pTarget,pColor);
}


void AGGLScreen::fillRect(const AGRect2 &pRect,const AGColor &c)
{
  AGGLPainter::fillRect(pRect,c);
}

void AGGLScreen::fillRects(const std::vector<std::pair<AGRect2,AGVector4> > &pRects)
{
  AGGLPainter::fillRects(pRects);
}


AGRect2 AGGLScreen::getRect() const
{
  return AGRect2(0,0,w,h);
}

void glColor(const AGColor &c)
{
  glColor4f(c.r/255.0,c.g/255.0,c.b/255.0,c.a/255.0);
}

void AGGLScreen::drawGradientAlpha(const AGRect2& pRect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  AGGLPainter::drawGradientAlpha(pRect,ul,ur,dl,dr);
}

void AGGLScreen::drawBorder(const AGRect2& rect,int W, const AGColor& c1, const AGColor& c2)
{
  AGGLPainter::drawBorder(rect,W,c1,c2);
}

void AGGLScreen::putPixel(int x,int y,const AGColor &pc)
{
  AGGLPainter::putPixel(x,y,pc);
}

void AGGLScreen::drawGradient(const AGRect2& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  drawGradientAlpha(rect,ul,ur,dl,dr);
}

void AGGLScreen::drawLine(const AGVector2 &p0,const AGVector2 &p1,const AGColor &c)
{
  AGGLPainter::drawLine(p0,p1,c);
}

void AGGLScreen::blitTri(const AGTexture &pSource,const AGTriangle2 &pSrc,const AGTriangle2 &pDest)
{
  AGGLPainter::blitTri(pSource,pSrc,pDest);
}

size_t AGGLScreen::getWidth() const
{
  return w;
}
size_t AGGLScreen::getHeight() const
{
  return h;
}
void AGGLScreen::clip(const AGRect2 &r)
{
  return;
#warning "insert clipping facility for opengl"

  AGRect2 x=AGRect2(0,0,w,h);
  AGRect2 m=x.intersect(r);

  glScissor((GLint)(m.x0()),
	    (GLint)(h-1-m.y1()),
	    (GLint)(m.width()),
	    (GLint)(m.height()));
  assertGL;
  glEnable(GL_SCISSOR_TEST);
  assertGL;
}
void AGGLScreen::unclip()
{
  glDisable(GL_SCISSOR_TEST);
  assertGL;
}


void AGGLScreen::beginPaint()
{
  //  CTRACE;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,getHeight()+1,0);
  glScalef(1,-1,1);
}

void AGGLScreen::endPaint()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}




bool opengl()
{
  return dynamic_cast<AGGLScreen*>(&getScreen());
}
