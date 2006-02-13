/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_gltexture.cc
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

#include <stdexcept>
#include "ag_gltexture.h"
#include "ag_surfacemanager.h"
#include "ag_rendercontext.h"
#include "ag_debug.h"

AGGLTexture::AGGLTexture(size_t W,size_t H,GLint format):w(W),h(H),d(1),m3d(false)
{
  getSurfaceManager()->registerMe(this);
  //  CTRACE;
  glGenTextures( 1, &mID);
  glBindTexture( GL_TEXTURE_2D,mID);

  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  /*
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);*/
  assertGL;
}
AGGLTexture::AGGLTexture(size_t W,size_t H,size_t D,GLint format):w(W),h(H),d(D),m3d(true)
{
  getSurfaceManager()->registerMe(this);
  {
    //    CTRACE;
  }
  glGenTextures( 1, &mID);
  glBindTexture( GL_TEXTURE_3D,mID);

  glTexImage3D(GL_TEXTURE_3D, 0, format, w, h, d, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, 0);

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  /*
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);*/
  assertGL;
}

AGGLTexture::~AGGLTexture()
{
  glDeleteTextures(1,&mID);
  assertGL;
  getSurfaceManager()->deregisterMe(this);
}

void AGGLTexture::setSurface(AGInternalSurface *pSurface,const AGVector2 &offset)
{
  CTRACE;
  GLint format;

  SDL_Surface *surface=pSurface->surface;

  switch(surface->format->BytesPerPixel)
    {
    case 3:
      format = GL_RGB;break;
    case 4:
      format = GL_RGBA;break;
    default:
      throw std::runtime_error("Surface-format not supported for texturing!");
    }

  AGRenderContext c;
  c.setTexture(this);
  c.begin();
  assertGL;
  
  /*
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  assertGL;
  glPixelStorei(GL_UNPACK_ROW_LENGTH,
                surface->pitch / surface->format->BytesPerPixel);
  */

  if(m3d)
    {
      int mw=std::min(surface->w,surface->h);
      int mh=std::max(surface->w,surface->h)/mw;
      glTexSubImage3D(GL_TEXTURE_3D,0,int(offset[0]),int(offset[1]),0,
		      mw,mw,mh,format,GL_UNSIGNED_BYTE,surface->pixels);
    }
  else
    {
      assertGL;
      
      cdebug(offset);
      cdebug(surface->w<<"  "<<surface->h);
      cdebug(w<<"  "<<h);
      cdebug(surface);
      cdebug(surface->pixels);
      glTexSubImage2D(GL_TEXTURE_2D, 0, int(offset[0]), int(offset[1]),
		      surface->w, surface->h, format, GL_UNSIGNED_BYTE,
		      surface->pixels);
    }
  assertGL;
}

GLuint AGGLTexture::id()
{
  return mID;
}
bool AGGLTexture::is3d() const
{
  return m3d;
}

size_t AGGLTexture::width() const
{
  return w;
}
size_t AGGLTexture::height() const
{
  return h;
}
size_t AGGLTexture::depth() const
{
  return d;
}
