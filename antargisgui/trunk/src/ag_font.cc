#include "ag_font.h"
#include "ag_fontengine.h"

AGFont::AGFont(const std::string &pFile,int size):
  mName(pFile),mSize(size),mColor(0xFF,0xFF,0xFF)
{
}

AGFont::AGFont()
{
  mName="Arial.ttf";
  mSize=14;
  mColor=AGColor(0xFF,0xFF,0xFF);
}

void AGFont::setColor(const AGColor &pColor)
{
  mColor=pColor;
}
AGColor AGFont::getColor() const
{
  return mColor;
}

void AGFont::setAlpha(Uint8 pAlpha)
{
  mAlpha=pAlpha;
}
Uint8 AGFont::getAlpha() const
{
  return mAlpha;
}

void AGFont::setSize(Uint8 pSize)
{
  mSize=pSize;
}
Uint8 AGFont::getSize() const
{
  return mSize;
}

void AGFont::setStyle(const Style &s)
{
  mStyle=s;
}

AGFont::Style AGFont::getStyle() const
{
  return mStyle;
}

void AGFont::setName(const std::string &s)
{
  mName=s;
}
std::string AGFont::getName() const
{
  return mName;
}

int AGFont::getWidth(const std::string &mText) const
{
  return AGFontEngine::getWidth(*this,mText);
}
int AGFont::getHeight(const std::string &mText) const
{
  return AGFontEngine::getHeight(*this,mText);
}


bool operator<(const AGFont&f1,const AGFont &f2)
{
  return f1.mStyle<f2.mStyle || 
    (f1.mStyle==f2.mStyle && (f1.mName<f2.mName || 
			      (f1.mName==f2.mName && (f1.mSize<f2.mSize ||
						      (f1.mSize==f2.mSize && (f1.mAlpha<f2.mAlpha ||
									      (f1.mAlpha==f2.mAlpha && f1.mColor<f2.mColor)))))));
}
