#include "ag_theme.h"
#include "ag_debug.h"

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
  mColors[pName]=pColor;
  //  cout<<"setting:"<<pName<<":"<<pColor.toString()<<endl;
}


AGFont AGTheme::getFont(const std::string &pName)
{
  if(mFonts.find(pName)==mFonts.end())
    return mFonts[trunk(pName)];
  return mFonts[pName];
}
AGColor AGTheme::getColor(const std::string &pName)
{
  if(mColors.find(pName)==mColors.end())
    return mColors[trunk(pName)];
  return mColors[pName];
}

std::string AGTheme::trunk(std::string s)
{
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



AGTheme *getDefaultTheme()
{
  AGTheme *t=new AGTheme;
  debug(t);
  return t;
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
    mTheme=getDefaultTheme();
  return mTheme;
}

bool AGTheme::hasSurface(const std::string &pName) const
{
  return(mSurfaces.find(pName)!=mSurfaces.end());
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
