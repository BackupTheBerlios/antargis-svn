/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_fbo.cc
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
#include "ag_fbo.h"
#include "ag_surface.h"
#include "ag_debug.h"
#include "ag_config.h"

#include <stdexcept>

#define USE_FBO

AGFBO::AGFBO(AGGLTexture *pTexture, bool withDepth):
  mTexture(pTexture),
  mWithDepth(withDepth)
{
  mTextureID=mTexture->id();
  w=mTexture->width();
  h=mTexture->height();

  init();
}


// init for depth-only writing
AGFBO::AGFBO(GLuint pTexture, size_t pW,size_t pH):
  mTexture(0),mTextureID(pTexture),
  mWithDepth(false)
{
  w=pW;
  h=pH;


  glGenFramebuffersEXT(1, &fb);
  if(mWithDepth)
    glGenFramebuffersEXT(1, &depth_rb);  // FIXME: this should read glGenRenderbuffersEXT
  
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  
  
  glBindTexture(GL_TEXTURE_2D,mTextureID);
  
  assertGL;
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, mTextureID, 0);
  assertGL;

  if(true)
  {
    // try add a renderbuffer as color attachment 
    unsigned int rb;

    glGenRenderbuffersEXT(1,&rb);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, w,h);//pTexture->width(), pTexture->height());
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, rb);

    
  }



  
  /*  if(mWithDepth)
    {
      throw std::runtime_error("AGFBO:not supported yet!");
      // initialize depth renderbuffer
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
      glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT16, w,h);//pTexture->width(), pTexture->height());
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
      }*/
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);


}


void AGFBO::init()
{
#ifdef USE_FBO
  glGenFramebuffersEXT(1, &fb);
  if(mWithDepth)
    glGenFramebuffersEXT(1, &depth_rb);
  
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  
  
  glBindTexture(GL_TEXTURE_2D,mTextureID);
  
  assertGL;
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, mTextureID, 0);
  assertGL;
  
  if(mWithDepth)
    {
      throw std::runtime_error("AGFBO:not supported yet!");
      // initialize depth renderbuffer
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
      glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT16, w,h);//pTexture->width(), pTexture->height());
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
    }
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  assertGL;
#endif
}

void AGFBO::beginDraw()
{
#ifdef USE_FBO
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  assertGL;

#endif
}

void AGFBO::endDraw()
{
#ifdef USE_FBO
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  assertGL;
      
#endif
}




#warning "add global var for this!"
bool canFBO()
{
#ifndef USE_FBO
  return false; // FIXME: FBO painting does not work yet
#else
  if(getConfig()->get("useFBO")=="")
    getConfig()->set("useFBO","true");

  if(getConfig()->get("useFBO")=="true")
    return GLEE_EXT_framebuffer_object;
  else
    return false;
#endif
}
