#include "ag_caption.h"

AGCaption::AGCaption(AGWidget *pParent,const AGRect &pRect,const std::string &pText,const AGFont &pFont,const AGBackground &pBG):
  AGText(pParent,pRect,pText,pFont),
  mBG(pBG)
{
}

void AGCaption::draw(const AGRect &pRect)
{
  // draw bg
  mBG.draw(pRect.project(getRect()));
  AGText::draw(pRect);
}

void AGCaption::setBackground(AGBackground pBG)
{
  mBG=pBG;
}
