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

void AGTheme::setFont(const std::string &pName,AGFont pFont)
{
  mFonts[pName]=pFont;
}
void AGTheme::setColor(const std::string &pName,AGColor pColor)
{
  cdebug(pName);
  mColors[pName]=pColor;
  //  cout<<"setting:"<<pName<<":"<<pColor.toString()<<endl;
}


AGFont AGTheme::getFont(const std::string &pName)
{
  cdebug(pName);
  if(mFonts.find(pName)==mFonts.end())
    return mFonts[trunk(pName)];
  return mFonts[pName];
}
AGColor AGTheme::getColor(const std::string &pName)
{
  //  cdebug(pName);
  if(mColors.find(pName)==mColors.end())
    return mColors[trunk(pName)];
  return mColors[pName];
}

std::string AGTheme::trunk(std::string s)
{
  //  cdebug(s);
  size_t i=s.find(".");
  if(i!=s.npos)
    s=s.substr(0,i);
  return s;
}

int AGTheme::getInt(const std::string &pName)
{
  if(mInts.find(pName)==mInts.end())
    return mInts[trunk(pName)];
  return mInts[pName];
}
void AGTheme::setInt(const std::string &pName,int i)
{
  mInts[pName]=i;
}



void loadDefaultTheme()
{
  loadThemeFile("theme.xml");
  
  if(!mTheme)
    mTheme=new AGTheme;

  debug(mTheme);
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

bool AGTheme::hasSurface(const std::string &pName) const
{
  //  cdebug(pName);
  return(mSurfaces.find(pName)!=mSurfaces.end());
}
bool AGTheme::hasColor(const std::string &pName) const
{
  //  cdebug(pName);
  return(mColors.find(pName)!=mColors.end());
}


AGSurface AGTheme::getSurface(const std::string &pName)
{
  if(mSurfaces.find(pName)==mSurfaces.end())
    return mSurfaces[trunk(pName)];
  return mSurfaces[pName];
}
void AGTheme::setSurface(const std::string &pName,const AGSurface &pSurface)
{
  assert(pSurface.valid());
  mSurfaces[pName]=pSurface;
  assert(mSurfaces[pName].valid());
}

void loadTheme(const xmlpp::Node&node,AGTheme &t,std::string name)
{
  xmlpp::Node::const_iterator i=node.begin();
  for(;i!=node.end();i++)
    {
      
      std::string n=name;
      if(n.length())
	n+=".";
      n+=(*i)->getName();
      if((*i)->get("name").length())
	{
	  std::string sname=name+"."+(*i)->get("name");
	  // read color /image whatever
	  if((*i)->getName()=="color")
	    t.setColor(sname,AGColor((*i)->get("color")));
	  if((*i)->getName()=="image")
	    t.setSurface(sname,getScreen().loadSurface((*i)->get("file")));
	  if((*i)->getName()=="value")
	    t.setInt(sname,toInt((*i)->get("value")));
	  if((*i)->getName()=="font")
	    {
	      AGFont f((*i)->get("file"),toInt((*i)->get("size")));
	      f.setColor(AGColor((*i)->get("color")));
	      t.setFont(sname,f);
	    }
	}
      else
	loadTheme(*i,t,n);
    }
}

void loadTheme(const std::string &pXML)
{
  AGTheme theme;

  xmlpp::Document p;
  p.parse_memory(pXML);

  xmlpp::Node n=p.root();

  loadTheme(n,theme,"");
  
  setTheme(theme);
}

bool loadThemeFile(const std::string &pFilename)
{
  AGTheme theme;

  xmlpp::Document p;
  if(!p.parseFile(pFilename))
    return false;

  xmlpp::Node n=p.root();

  loadTheme(n,theme,"");
  
  setTheme(theme);
  return true;
}

std::string addPoint(const std::string &pTheme)
{
  if(pTheme.length())
    if(pTheme[pTheme.length()-1]!='.')
      return pTheme+".";
  return pTheme;
}
