/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_button.cc
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

#include "ag_button.h"
#include "ag_draw.h"
#include "ag_edit.h"
#include "ag_theme.h"
#include "ag_debug.h"
#include "ag_image.h"
#include "ag_border.h"

using namespace std;

AGButton::AGButton(AGWidget *pParent,const AGRect &r,const std::string&pText,int id):
  AGWidget(pParent,r),
  mText(pText),mID(id),mState(NORMAL),mTextW(0)
{
  cdebug(r);
  //  lower=getTheme()->getInt("buttonLowerOnClick");
  //  borderWidth=getTheme()->getInt("button.border.width");
  //  addChild(new 
  setTheme("");
  AGFont font("Arial.ttf");
  font.setColor(AGColor(0,0,0));
  mTextW=new AGEdit(this,r.origin().shrink(borderWidth));//,mText,font);
  mTextW->setText(mText);
  mTextW->setAlign(EDIT_CENTER);
  mTextW->setVAlign(EDIT_VCENTER);
  mTextW->setFont(font);
  mTextW->setMutable(false);
  mTextW->setBackground(false);
  addChild(mTextW);

  setTheme("");
  mChecked=false;

  //  cdebug("borderWidth:"<<borderWidth);
  mEnabled=true;
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

void AGButton::draw(AGPainter &p)
{
  //  CTRACE;
  //  return;
  AGPainter p2(p);
  p2.transform(AGRect(0,0,width(),height()).shrink(borderWidth));
  mBG[mState].draw(p2);
  mBorder[mState].draw(p2);

  if(borderWidth==0)
    return;
  //  AGSurface s(getScreen());

  AGColor bc1;
  AGColor bc2;
  AGTheme *theme=getTheme();

  bc1=theme->getColor(mTheme+"button.border.color1");
  bc2=theme->getColor(mTheme+"button.border.color2");
  {
  
    //  CTRACE;

    //FIXME: use painter !!!!!!
    AGRect mr;//=r.project(getRect());
    //  cdebug(getRect());
    //  cdebug(mr);
    mr=getRect().origin();
    //  cdebug(mr);
    if(mState==NORMAL || mState==LIGHTED)
      p.drawBorder(mr,borderWidth,bc1,bc2);
    else 
      p.drawBorder(mr,borderWidth,bc2,bc1);
  }
}


bool AGButton::eventMouseEnter()
{
  if(!mEnabled)
    return false;
  if(mChecked)
    mState=CHECKEDLIGHTED;
  else
    mState=LIGHTED;
  return false;
}
bool AGButton::eventMouseLeave()
{
  if(!mEnabled)
    return false;
  if(mChecked)
    mState=CHECKED;
  else
    mState=NORMAL;
  return false;
}


bool AGButton::eventMouseButtonDown(const AGEvent *m)
{
  if(!mEnabled)
    return false;

  // CTRACE;
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
    {
      //      cdebug(getScreenRect());
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  mState=PRESSED;
	  //  ccdebug("Pressed");
	  //	  sigClick(m);
	  //	  cdebug(getName());
	  AGWidget::eventMouseButtonDown(m); // let it get the buttondown-info

	  return true;// FIXME:TEST false; // events only get eaten if in window
	}
    }
  return AGWidget::eventMouseButtonDown(m);
}

bool AGButton::eventMouseButtonUp(const AGEvent *m)
{
  if(!mEnabled)
    return false;
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
    {
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  if(mChecked)
	    mState=CHECKEDLIGHTED;
	  else
	    mState=LIGHTED;
	}
      else
	{
	  if(mChecked)
	    mState=CHECKED;
	  else
	    mState=NORMAL;
	}
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
std::string AGButton::getCaption() const
{
  return mText;
}

void AGButton::setEnabled(bool pEnable)
{
  mEnabled=pEnable;
}

void AGButton::setTheme(const std::string &pTheme)
{
  mTheme=addPoint(pTheme);
  cdebug(mTheme);

  lower=getTheme()->getInt(mTheme+"buttonLowerOnClick");
  borderWidth=getTheme()->getInt(mTheme+"button.border.width");

  mBG[NORMAL]=AGBackground(mTheme+"button.background.normal");
  mBG[PRESSED]=AGBackground(mTheme+"button.background.pressed");
  mBG[LIGHTED]=AGBackground(mTheme+"button.background.lighted");
  mBG[CHECKED]=AGBackground(mTheme+"button.background.checked");
  mBG[CHECKEDLIGHTED]=AGBackground(mTheme+"button.background.checkedlighted");

  mBorder[NORMAL]=AGBorder(mTheme+"button.border.normal");
  mBorder[PRESSED]=AGBorder(mTheme+"button.border.pressed");
  mBorder[LIGHTED]=AGBorder(mTheme+"button.border.lighted");
  mBorder[CHECKED]=AGBorder(mTheme+"button.border.checked");
  mBorder[CHECKEDLIGHTED]=AGBorder(mTheme+"button.border.checkedlighted");

  if(mTextW)
    mTextW->setTheme(mTheme+"button.text");
}

void AGButton::setCaption(const std::string &pCaption)
{
  mText=pCaption;
  if(mTextW)
    mTextW->setText(pCaption);
}

void AGButton::setState(const State &pState)
{
  mState=pState;
}

void AGButton::setChecked(bool pChecked)
{
  mChecked=pChecked;
  if(mChecked)
    {
      if(mState==LIGHTED || mState==CHECKEDLIGHTED)
	setState(CHECKEDLIGHTED);
      else
	setState(CHECKED);
    }
  else
    {
      if(mState==LIGHTED || mState==CHECKEDLIGHTED)
	setState(LIGHTED);
      else
	setState(NORMAL);
    }
}
bool AGButton::isChecked() const
{
  return mChecked;
}


AGButton &toAGButton(AGWidget &pWidget)
{
  return dynamic_cast<AGButton&>(pWidget);
}

