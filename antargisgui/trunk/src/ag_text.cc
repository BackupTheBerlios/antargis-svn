
#include "ag_text.h"
#include "ag_debug.h"
#include "ag_surface.h"
#include "ag_fontengine.h"
#include "ag_geometry.h"

#include <iostream>

using namespace std;

AGText::AGText(AGWidget *pParent,const AGRect &r,const std::string &pText,const AGFont &pFont):
  AGWidget(pParent,r),
  mText(pText),mFont(pFont)
{
  mFixedSize=true;
}

AGText::AGText(AGWidget *pParent,const AGPoint &p,const std::string &pText,const AGFont &pFont):
  AGWidget(pParent,AGRect(p.x,p.y,pFont.getWidth(pText),pFont.getHeight(pText))),
  mText(pText),mFont(pFont)
{
  mFixedSize=false;
}

void AGText::draw(const AGRect &r)
{
  //  CTRACE;
  //  cdebug(mText);

  //  AGSurface screen(getScreen());
  //  cout<<"width:"<<endl;
  // center

  AGRect r2=r.project(getRect());
  //  cout<<"r2:"<<r2.toString()<<endl;
  //  cout<<"gr2:"<<getRect().toString()<<endl;
  //  cdebug("gr:"<<getRect().toString()<<endl);
  //  cdebug("r2:"<<r2<<endl);

  int x=(r2.w-mFont.getWidth(mText))/2+r2.x;
  int y=(r2.h-mFont.getHeight(mText))/2+r2.y;
  //cout<<"x:"<<x<<" y:"<<y<<" text:"<<mText<<endl;
  //  cdebug(x<<"/"<<y);

  AGFontEngine::renderText(&getScreen(),r2,x,y,mText,mFont);

  /*
  AGSurface ms(width(),height());
  AGFontEngine::renderText (ms,AGRect(0,0,r2.w,r2.h),x,y,mText,mFont);// int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &pFont)
  getScreen()->blit(ms,r2);*/
}

void AGText::setDeriveRect()
{
  setRect(AGRect(getRect().x,getRect().y,mFont.getWidth(mText),mFont.getHeight(mText)));
}

void AGText::setText(const std::string &pText)
{
  mText=pText;
  if(!mFixedSize)
    {
      setWidth(mFont.getWidth(pText));
      setHeight(mFont.getHeight(pText));
    }
}
std::string AGText::getText() const
{
  return mText;
}
