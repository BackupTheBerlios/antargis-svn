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

#include "ag_geometry.h"
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
  SDL_GL_SwapBuffers();
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

  glEnable(GL_DEPTH_TEST); // enable depth test
  glDepthFunc(GL_LEQUAL); // set type depth test
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // GL_NICEST // best perspective correction
  glEnable(GL_BLEND);

  glViewport( 0, 0, w, h );
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
  //  CTRACE;
  if(!inScreen(pRect))
    return;

  int w2=std::min((int)pRect.w(),pSource.width());
  int h2=std::min((int)pRect.h(),pSource.height());

  float x0=pRect.x();
  float y0=h-pRect.y();
  float x1=pRect.x()+w2;
  float y1=h-(pRect.y()+h2);

  AGRenderContext context;
  context.setCulling(false);
  context.setTexture(const_cast<AGTexture&>(pSource).glTexture());
  context.setDepthTest(false);
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


void AGGLScreen::fillRect(const AGRect2 &pRect,const AGColor &c)
{
  float x0=pRect.x();
  float y0=h-pRect.y();
  float x1=pRect.x()+pRect.w();
  float y1=h-pRect.y()-pRect.h();

  //  cdebug(pRect<<"  "<<c);
  AGRenderContext context;
  context.setColor(c);
  context.setCulling(false);
  context.setAlpha(0,GL_NONE);
  context.setDepthTest(false);
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
  float x0=pRect.x();
  float y0=(h)-pRect.y();
  float x1=pRect.x()+pRect.w();
  float y1=(h)-(pRect.y()+pRect.h());

  // turned
  AGRenderContext context;
  context.setColor(AGVector4(1,1,1,1));
  context.setCulling(false);
  context.setDepthTest(false);

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

void AGGLScreen::drawBorder(const AGRect2& rect,int W, const AGColor& c1, const AGColor& c2)
{

  float x0=rect.x();
  float y0=h-1-rect.y();
  float x1=rect.x()+rect.w();
  float y1=h-1-(rect.y()+rect.h());

  AGRenderContext context;
  context.setColor(c1);
  context.setCulling(false);
  context.setDepthTest(false);
  context.begin();

  // left and //top
  //  glColor(c1);
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

  //  glCullFace(GL_BACK);
  //  glEnable(GL_CULL_FACE);


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

void AGGLScreen::drawGradient(const AGRect2& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  drawGradientAlpha(rect,ul,ur,dl,dr);
}

void AGGLScreen::drawLine(const AGVector2 &p0,const AGVector2 &p1,const AGColor &c)
{
  AGRenderContext context;
  context.setColor(c);
  context.setDepthTest(false);
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
  c.setDepthTest(false);
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
void AGGLScreen::clip(const AGRect2 &r)
{
  AGRect2 x=AGRect2(0,0,w,h);
  AGRect2 m=x.intersect(r);
  //  cdebug(r<<"   "<<m<<"  "<<x);


  //  cdebug(m.x0()<<"  "<<h-1-m.y0()<<"  "<<m.width()<<"  "<<m.height());

  glScissor((GLint)(m.x0()),
	    (GLint)(h-1-m.y1()),
	    (GLint)(m.width()),
	    (GLint)(m.height()));
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
void AGGLScreen::tile(const AGTexture &pSource,const AGRect2 &pDest,const AGRect2 &pSrc)
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

  AGRect2 d(100,100,100,100);

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
