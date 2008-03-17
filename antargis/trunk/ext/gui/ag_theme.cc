/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_theme.cc
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

#include "ag_theme.h"
#include "ag_debug.h"
#include "ag_xml.h"
#include "ag_tools.h"
#include "ag_config.h"

#include <iostream>
using namespace std;

AGTheme *mTheme=0;

AGTheme::AGTheme()
  {
  }

AGTheme::~AGTheme()
  {
  }

void AGTheme::setFont(const AGString &pName,AGFont pFont)
  {
    mFonts[pName]=pFont;
  }
void AGTheme::setColor(const AGString &pName,AGColor pColor)
  {
    //  cdebug(pName);
    mColors[pName]=pColor;
    //  cout<<"setting:"<<pName<<":"<<pColor.toString()<<endl;
  }


AGFont AGTheme::getFont(const AGString &pName)
  {
    //  cdebug(pName<<":"<<mFonts[pName].toString());
    if(mFonts.find(pName)==mFonts.end())
      return mFonts[trunk(pName)];
    return mFonts[pName];
  }
AGColor AGTheme::getColor(const AGString &pName)
  {
    //  cdebug(pName);
    if(mColors.find(pName)==mColors.end())
      return mColors[trunk(pName)];
    return mColors[pName];
  }

AGString AGTheme::trunk(AGString s)
  {
    //  cdebug(s);
    size_t i=s.find(".");
    if(i!=s.npos)
      s=s.substr(0,i);
    return s;
  }

int AGTheme::getInt(const AGString &pName)
  {
    if(mInts.find(pName)==mInts.end())
      return mInts[trunk(pName)];
    return mInts[pName];
  }
void AGTheme::setInt(const AGString &pName,int i)
  {
    mInts[pName]=i;
  }



void loadDefaultTheme()
  {
    loadThemeFile("theme.xml");

    if(!mTheme)
      mTheme=new AGTheme;
  }

void setTheme(const AGTheme &t)
  {
    if(mTheme)
      delete mTheme;
    mTheme=new AGTheme(t);
  }

AGTheme *getTheme()
  {
    if(mTheme==0)
      loadDefaultTheme();
    return mTheme;
  }

bool AGTheme::hasSurface(const AGString &pName) const
{
  //  cdebug(pName);
  return(mSurfaces.find(pName)!=mSurfaces.end());
}
bool AGTheme::hasColor(const AGString &pName) const
{
  //  cdebug(pName);
  return(mColors.find(pName)!=mColors.end());
}


AGSurface AGTheme::getSurface(const AGString &pName)
  {
    if(mSurfaces.find(pName)==mSurfaces.end())
      return mSurfaces[trunk(pName)];
    return mSurfaces[pName];
  }
void AGTheme::setSurface(const AGString &pName,const AGSurface &pSurface)
  {
    assert(pSurface.valid());
    mSurfaces[pName]=pSurface;
    assert(mSurfaces[pName].valid());
  }

std::string AGTheme::getSurfaceName(const AGString &pName)
  {
    if(mSurfaceNames.find(pName)==mSurfaceNames.end())
      return mSurfaceNames[trunk(pName)];
    return mSurfaceNames[pName];
  }
void AGTheme::setSurfaceName(const AGString &pName,const std::string &pSurface)
  {
    mSurfaceNames[pName]=pSurface;
  }


void loadTheme(const Node&node,AGTheme &t,AGString name)
  {
    Node::const_iterator i=node.begin();
    for(;i!=node.end();i++)
      {

        AGString n=name;
        if(n.length())
          n+=".";
        n+=(*i)->getName();
        if((*i)->get("name").length())
          {
            AGString sname=name+"."+(*i)->get("name");
            // read color /image whatever
            if((*i)->getName()=="color")
              t.setColor(sname,AGColor((*i)->get("color")));
            if((*i)->getName()=="image")
              {
                t.setSurface(sname,AGSurface::load((*i)->get("file")));
                t.setSurfaceName(sname,(*i)->get("file"));
              }
            if((*i)->getName()=="value")
              t.setInt(sname,(*i)->get("value").toInt());
            if((*i)->getName()=="font")
              {
                AGFont f((*i)->get("file"),(*i)->get("size").toInt());
                f.setColor(AGColor((*i)->get("color")));
                f.setEmbossed((*i)->get("embossed")=="true");
                f.setInset((*i)->get("inset")=="true");
                if((*i)->get("style")=="bold")
                  f.setStyle(AGFont::BOLD);
                f.setBorderColor(AGColor((*i)->get("borderColor")));
                f.setBorder((*i)->get("borderWidth").toInt());

                t.setFont(sname,f);
              }
          }
        else
          loadTheme(**i,t,n);
      }
  }

void loadTheme(const AGString &pXML)
  {
    AGTheme theme;

    Document p;
    p.parseMemory(pXML);

    Node n=p.root();

    loadTheme(n,theme,"");

    setTheme(theme);
  }

bool loadThemeFile(const AGString &pFilename)
  {
    AGTheme theme;

    Document p;
    if(!p.parseFile(pFilename))
      return false;

    const Node &n=p.root();

    loadTheme(n,theme,"");

    setTheme(theme);
    return true;
  }

AGString addPoint(const AGString &pTheme)
  {
    if(pTheme.length())
      if(pTheme[pTheme.length()-1]!='.')
        return pTheme+".";
    return pTheme;
  }
