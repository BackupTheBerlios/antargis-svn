#include "ag_frame.h"

AGFrame::AGFrame(AGWidget *pParent,const AGRect &pRect,size_t width):AGWidget(pParent,pRect),
								     mWidth(width),mBorder(0)
{
}
AGFrame::AGFrame(AGWidget *pParent,const AGRect &pRect,const AGBorder &pBorder):AGWidget(pParent,pRect),
										mWidth(pBorder.getWidth()),mBorder(new AGBorder(pBorder))
{
}

AGFrame::~AGFrame()
{
  delete mBorder;
}

AGRect AGFrame::getClientRect() const
{
  return getRect().origin().shrink(mWidth);
}

void AGFrame::draw(AGPainter &p)
{
  if(mBorder)
    mBorder->draw(p);
}
