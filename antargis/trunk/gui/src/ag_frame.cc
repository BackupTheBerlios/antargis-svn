#include "ag_frame.h"
#include "ag_screen.h"

AGFrame::AGFrame(AGWidget *pParent,const AGRect2 &pRect,size_t pWidth):AGWidget(pParent,pRect),
								      mWidth(pWidth),mBorder(0),mTexture(width(),height())
{
  mTextureInited=false;
  mUseTexture=true;
}
AGFrame::AGFrame(AGWidget *pParent,const AGRect2 &pRect,const AGBorder &pBorder):AGWidget(pParent,pRect),
										 mWidth(pBorder.getWidth()),mBorder(new AGBorder(pBorder)),mTexture(width(),height())
{
  mTextureInited=false;
  mUseTexture=true;
}

AGFrame::~AGFrame()
{
  delete mBorder;
}

AGRect2 AGFrame::getClientRect() const
{
  return getRect().origin().shrink(mWidth);
}

void AGFrame::prepareDraw()
{
  if(mUseTexture && mBorder)
    {
      if(!mTextureInited)
	{
	  AGPainter p(mTexture);
	  mBorder->draw(p);
	  mTextureInited=true;

	}
    }
  AGWidget::prepareDraw();
}

void AGFrame::draw(AGPainter &p)
{
  if(mBorder)
    {
      if(mUseTexture && mTextureInited)
	p.blit(mTexture,mTexture.getRect());
      else
	mBorder->draw(p);
    }
}
