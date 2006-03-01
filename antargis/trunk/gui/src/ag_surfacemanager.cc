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

#include "ag_surfacemanager.h"
#include "ag_rendercontext.h"
#include "ag_surface.h"
#include "ag_texture.h"
#include "ag_kill.h"
#include "ag_debug.h"

#include <map>

////////////////////////////////////////////////////////////////////////
// AGSurfaceManager
////////////////////////////////////////////////////////////////////////

AGSurfaceManager *mSurfaceManager=0;
bool mSurfaceManagerDeleted=false;

AGSurfaceManager::AGSurfaceManager()
{
}
AGSurfaceManager::~AGSurfaceManager()
{
  for(std::set<AGInternalSurface*>::iterator i=mSDLSurfaces.begin();i!=mSDLSurfaces.end();++i)
    {
      AGFreeSurface((*i)->surface);
      delete *i;
    }
  for(std::set<AGGLTexture*>::iterator i=mGLTextures.begin();i!=mGLTextures.end();++i)
    {
      delete *i;
    }

  mSurfaceManagerDeleted=true;
  mSurfaceManager=0;
}

void AGSurfaceManager::clear()
{
  cleanup(true);
  cleanup(true);
  cleanup(true);
}


void AGSurfaceManager::registerMe(AGSurface *pSurface)
{
  mSurfaces.insert(pSurface);
}
void AGSurfaceManager::deregisterMe(AGSurface *pSurface)
{
  mSurfaces.erase(pSurface);
}

void AGSurfaceManager::registerMe(AGTexture *pTexture)
{
  mTextures.insert(pTexture);
}
void AGSurfaceManager::deregisterMe(AGTexture *pTexture)
{
  mTextures.erase(pTexture);
}

void AGSurfaceManager::registerMe(AGInternalSurface *p)
{
  mSDLSurfaces.insert(p);
}
void AGSurfaceManager::deregisterMe(AGInternalSurface *p)
{
  mSDLSurfaces.erase(p);
}

void AGSurfaceManager::registerMe(AGRenderContext *p)
{
  mRContext.insert(p);
}
void AGSurfaceManager::deregisterMe(AGRenderContext *p)
{
  mRContext.erase(p);
}

void AGSurfaceManager::registerMe(AGGLTexture *p)
{
  mGLTextures.insert(p);
}
void AGSurfaceManager::deregisterMe(AGGLTexture *p)
{
  mGLTextures.erase(p);
}

AGSurfaceManager *getSurfaceManager()
{
  if(mSurfaceManager==0)
    {
      if(!mSurfaceManagerDeleted)
	{
	  mSurfaceManager=new AGSurfaceManager();
	  
	  REGISTER_SINGLETON(mSurfaceManager);
	}
    }
  return mSurfaceManager;
}


void AGSurfaceManager::cleanup(bool force)
{
  //return;
  size_t oldTexMem=getUsedTexMem();
  if(oldTexMem<16000000 && !force)
    return;
  cdebug("oldTexMem:"<<oldTexMem);
  
  /*
  cdebug("gltex:"<<mGLTextures.size());
  cdebug("tex:"<<mTextures.size());
  cdebug("sfs:"<<mSurfaces.size());
  cdebug("sdl-sfs:"<<mSDLSurfaces.size());
  cdebug("rts:"<<mRContext.size());
  */
  // (1) first cleanup textures
  //   (i)   get used AGTextures and store used AGGLTextures
  //   (ii)  clear used-flags
  //   (iii) remove unused AGGLTextures
  //   (iv)  remove glTexture Pointers from internalSurfaces, where removed
  // (2) cleanup internal-surfaces
  //   (i)   iterate through AGTextures & AGSurfaces and get all used AGInternalSurfaces
  //   (ii)  delete unused


  #warning "// ALSO FOR SDL-MODE!!"

  std::set<AGGLTexture*> usedGL;
  for(std::set<AGTexture*>::iterator i=mTextures.begin();i!=mTextures.end();i++)
    {
      if((*i)->textureUsed())
	{
	  usedGL.insert((*i)->glTexture());
	  (*i)->clearTextureUsed();
	}
      else
	(*i)->clearTexture();
    }

  for(std::set<AGRenderContext*>::iterator i=mRContext.begin();i!=mRContext.end();i++)
    {
      if((*i)->getTexture())
	usedGL.insert((*i)->getTexture());
    }

  // build map
  std::map<AGGLTexture*,AGInternalSurface*> smap;
  for(std::set<AGInternalSurface*>::iterator i=mSDLSurfaces.begin();i!=mSDLSurfaces.end();++i)
    {
      if(*i && (*i)->glTexture)
	smap.insert(std::make_pair((*i)->glTexture,*i));
    }

  std::list<AGGLTexture*> gls;
  std::copy(mGLTextures.begin(),mGLTextures.end(),std::back_inserter(gls));

  size_t texMem=0;

  for(std::list<AGGLTexture*>::iterator i=gls.begin();i!=gls.end();)
    {
      if(usedGL.find(*i)==usedGL.end())
	{
	  std::map<AGGLTexture*,AGInternalSurface*>::iterator l=smap.find(*i);
	  if(l!=smap.end())
	    l->second->glTexture=0;
	  delete *i;
	  i=gls.erase(i);
	  cdebug("yay");
	}
      else
	{
	  texMem+=(*i)->width()*(*i)->height()*(*i)->depth()*4;
	  i++;
	}
    }
  cdebug("texMem:"<<texMem);
  
  mGLTextures.clear();
  std::copy(gls.begin(),gls.end(),std::inserter(mGLTextures,mGLTextures.begin()));

#warning "optimize this - it really takes some time!!!"
}



size_t AGSurfaceManager::getUsedTexMem() const
{
  size_t texMem=0;
  for(std::set<AGGLTexture*>::const_iterator i=mGLTextures.begin();i!=mGLTextures.end();i++)
    texMem+=(*i)->width()*(*i)->height()*(*i)->depth()*4;
  return texMem;
}
