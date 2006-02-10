/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_rendercontext.cc
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

#include "ag_rendercontext.h"
#include "ag_debug.h"
#include "ag_texture.h"
#include "ag_surfacemanager.h"

AGRenderContext gStandardContext;
AGRenderContext *gCurrentContext=&gStandardContext;

AGRenderContext::AGRenderContext():
  mColor(0),
  mTexture(0),
  mLighting(false),
  mDepthWrite(true),
  mAlphaThres(-1),
  mAlpha(GL_NONE),
  mCulling(true)
{
  getSurfaceManager()->registerMe(this);
}

AGRenderContext::AGRenderContext(const AGRenderContext &c):
  mColor(c.mColor?new AGVector4(*c.mColor):0),
  mTexture(c.mTexture),
  mLighting(c.mLighting),
  mDepthWrite(c.mDepthWrite)
{
  getSurfaceManager()->registerMe(this);
  //  if(mTexture)
  //    mTexture->incRef();
}

AGRenderContext::~AGRenderContext()
{
  if(gCurrentContext==this)
    {
      //CTRACE;
      // disable
      //      end();
      throw std::runtime_error("Current rendercontext deleted!");
    }

  /*
  if(mTexture)
    {
      //      mTexture->decRef();
      deleteChecked(mTexture);
    }
  */
  delete mColor;
  getSurfaceManager()->deregisterMe(this);
}

AGRenderContext *AGRenderContext::getCurrent()
{
  return gCurrentContext;
}


void AGRenderContext::begin()
{
  // color setting
  if(mColor!=gCurrentContext->mColor)
    {
      if(mColor)
	{
	  glEnable(GL_COLOR_MATERIAL);
	  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	}
      else
	glDisable(GL_COLOR_MATERIAL);
    }
  if(mColor)
    {
      glColor4fv(*mColor);
      //      CTRACE;
    }

  bool t=false;


  #warning "add missing features! (depthwrite,alpha,culling)"


  if(mTexture==gCurrentContext->mTexture)
    {
      // early out
    }
  else
    {
      AGGLTexture *my=mTexture;
      AGGLTexture *last=0;
      if(gCurrentContext->mTexture)
	last=gCurrentContext->mTexture;
      /*

      cdebug("tex:"<<mTexture<<"   "<<gCurrentContext->mTexture);
      if(mTexture && gCurrentContext->mTexture)
	cdebug("tex:"<<mTexture->getTextureID()<<"   "<<gCurrentContext->mTexture->getTextureID());

      */
      // texture setting
      if(!mTexture)
	{
	  if(last)
	    {
	      if(last->is3d())
		glDisable(GL_TEXTURE_3D);
	      else
		glDisable(GL_TEXTURE_2D);
	      t=true;
	    }
	}
      else
	{
	  if(!last)
	    {
	      // simply enable
	      if(my->is3d())
		{
		  glEnable(GL_TEXTURE_3D);
		  glBindTexture(GL_TEXTURE_3D,my->id());
		}
	      else
		{
		  glEnable(GL_TEXTURE_2D);
		  glBindTexture(GL_TEXTURE_2D,my->id());
		}
	      t=true;
	    }
	  else
	    {
	      if(my->is3d())
		{
		  if(!last->is3d())
		    {
		      glDisable(GL_TEXTURE_2D);
		      glEnable(GL_TEXTURE_3D);
		    }
		  if(last->id()!=my->id())
		    glBindTexture(GL_TEXTURE_3D,my->id());
		}
	      else
		{
		  if(last->is3d())
		    {
		      glDisable(GL_TEXTURE_3D);
		      glEnable(GL_TEXTURE_2D);
		    }
		  if(last->id()!=my->id())
		    {
		      glBindTexture(GL_TEXTURE_2D,my->id());
		      //		      cdebug(mTexture->id());
		    }
		}
	    }
	}
    }  


  // lighting
  if(mLighting!=gCurrentContext->mLighting)
    {
      if(mLighting)
	glEnable(GL_LIGHTING);
      else
	glDisable(GL_LIGHTING);
    }

  if(gCurrentContext!=&gStandardContext)
    {
      AGRenderContext *d=gCurrentContext;
      gCurrentContext=0;
      delete d;
    }
  gCurrentContext=new AGRenderContext(*this);
}
void AGRenderContext::end()
{
  if(mColor)
    {
      glDisable(GL_COLOR_MATERIAL);
    }
  if(mTexture)
    {
      if(mTexture->is3d())
	glDisable(GL_TEXTURE_3D);
      else
	glDisable(GL_TEXTURE_2D);
    }

  gCurrentContext=&gStandardContext;
}

void AGRenderContext::setColor(const AGVector4 &pColor)
{
  mColor=new AGVector4(pColor);
}
void AGRenderContext::setColor(const AGColor &pColor)
{
  float f=1.0/255.0f;
  mColor=new AGVector4(pColor.r*f,
		       pColor.g*f,
		       pColor.b*f,
		       pColor.a*f);
}


void AGRenderContext::setTexture(AGGLTexture *pTexture)
{
  /*  if(mTexture)
    {
      mTexture->decRef();
      deleteChecked(mTexture);
      }*/
  mTexture=pTexture;
  //  mTexture->incRef();
}

void AGRenderContext::setLighting(bool pLight)
{
  mLighting=pLight;
}

void AGRenderContext::setDepthWrite(bool w)
{
  mDepthWrite=w;
}
void AGRenderContext::setCulling(bool c)
{
  mCulling=c;
}
void AGRenderContext::setAlpha(float v,GLuint g)
{
  mAlpha=g;
  mAlphaThres=v;
}

AGGLTexture *AGRenderContext::getTexture()
{
  return mTexture;
}
