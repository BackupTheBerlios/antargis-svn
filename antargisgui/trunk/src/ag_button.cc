#include "ag_button.h"
#include "ag_draw.h"
#include "ag_text.h"
#include "ag_theme.h"
#include "ag_debug.h"
#include "ag_image.h"

using namespace std;

AGButton::AGButton(AGWidget *pParent,const AGRect &r,const std::string&pText,int id,const std::string&pStyle):
  AGWidget(pParent,r),
  mText(pText),mID(id),mStyle(pStyle),mState(NORMAL),mTextW(0)
{
  lower=getTheme()->getInt("buttonLowerOnClick");
  borderWidth=getTheme()->getInt("borderWidth.button");
  //  addChild(new 
  AGFont font("Arial.ttf");
  font.setColor(AGColor(0,0,0));
  mTextW=new AGText(this,r.origin().shrink(borderWidth),mText,font);
  addChild(mTextW);

  mBG[NORMAL]=AGBackground("buttonbg.normal");
  mBG[PRESSED]=AGBackground("buttonbg.pressed");
  mBG[LIGHTED]=AGBackground("buttonbg.lighted");
  mBG[CHECKED]=AGBackground("buttonbg.checked");

  cdebug("borderWidth:"<<borderWidth);
}

void AGButton::setSurface(AGSurface pSurface,bool pChangeSize)
{
  mSurface=pSurface;
  AGImage *image=new AGImage(this,AGPoint(0,0),mSurface,false);
  addChild(image);
  mTextW->hide();
  if(pChangeSize)
    {
      setWidth(image->width()+2+2*borderWidth);
      setHeight(image->height()+2+2*borderWidth);
    }

  else
    {
      // then center
      image->setLeft((width()-image->width())/2);
      image->setTop((height()-image->height())/2);
    }
}


/*AGButton::AGButton():AGWidget(0,AGRect(0,0,0,0))
{
}*/

void AGButton::draw(const AGRect &r)
{
  CTRACE;
  return;
  mBG[mState].draw(r.project(getRect().shrink(borderWidth)));
  //  AGSurface s(getScreen());

  AGColor bc1;
  AGColor bc2;
  AGTheme *theme=getTheme();

  std::string style;
  if(mState==NORMAL)
    style=".normal";
  else if(mState==LIGHTED)
    style=".lighted";
  else
    style=".pressed";


  bc1=theme->getColor(std::string("borderColor1")+style);
  bc2=theme->getColor(std::string("borderColor2")+style);
{
  
  CTRACE;
  AGRect mr=r.project(getRect());
  if(mState==NORMAL || mState==LIGHTED)
    AGDraw::drawBorder(&getScreen(),mr,borderWidth,bc1,bc2);
  else 
    AGDraw::drawBorder(&getScreen(),mr,borderWidth,bc2,bc1);
    }
  //  drawBackground(r);
}

/*
void AGButton::drawBackground(const AGRect &r)
{
  //  cdebug("r:"<<r<<" mr:"<<getRect());
  AGColor c1,c2,c3,c4;
  // theme background-image - evtl.

  AGTheme *theme=getTheme();

  std::string style;
  if(mState==NORMAL)
    style=".normal";
  else if(mState==LIGHTED)
    style=".lighted";
  else
    style=".pressed";

      // border color
  AGColor bc1;
  AGColor bc2;

  c1=theme->getColor(std::string("gradientColor1")+style);
  c2=theme->getColor(std::string("gradientColor2")+style);
  c3=theme->getColor(std::string("gradientColor3")+style);
  c4=theme->getColor(std::string("gradientColor4")+style);


  bc1=theme->getColor(std::string("borderColor1")+style);
  bc2=theme->getColor(std::string("borderColor2")+style);

  AGSurface s(getScreen());
  
  AGRect mr=r.project(getRect());

  AGDraw::drawGradient(s,mr.shrink(borderWidth),c1,c2,c3,c4);
  if(mState==NORMAL || mState==LIGHTED)
    AGDraw::drawBorder(s,mr,borderWidth,bc1,bc2);
  else 
    AGDraw::drawBorder(s,mr,borderWidth,bc2,bc1);
}
*/
bool AGButton::eventMouseEnter()
{
  mState=LIGHTED;
  return false;
}
bool AGButton::eventMouseLeave()
{
  mState=NORMAL;
  return false;
}


bool AGButton::eventMouseButtonDown(const AGEvent *m)
{
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
    {
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  mState=PRESSED;
	  ccdebug("Pressed");
	  sigClick(m);
	  return true;
	}
    }
  return AGWidget::eventMouseButtonDown(m);
}

bool AGButton::eventMouseButtonUp(const AGEvent *m)
{
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
    {
      if(getScreenRect().contains(e->getMousePosition()))
	mState=LIGHTED;
      else
	mState=NORMAL;
    }
  return AGWidget::eventMouseButtonUp(m);
}

void AGButton::setWidth(int w)
{
  ccdebug(w);
  AGWidget::setWidth(w);
  std::list<AGWidget*>::iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    (*i)->setRect(getRect().shrink(borderWidth));
}
void AGButton::setHeight(int h)
{
  AGWidget::setHeight(h);
  std::list<AGWidget*>::iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    (*i)->setRect(getRect().shrink(borderWidth));
}
/*
AGButton AGButton::test()
{
  cdebug("test"<<endl);
  AGButton b(0,AGRect(0,0,0,0),"test");
  return b;
}
*/
std::string AGButton::getName() const
{
  return mText;
}
