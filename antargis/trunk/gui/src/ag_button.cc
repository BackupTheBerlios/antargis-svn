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
#include "ag_glsurface.h"

using namespace std;

AGButton::AGButton(AGWidget *pParent,const AGRect2 &r,const std::string&pText,int id):
  AGWidget(pParent,r),
  mText(pText),mID(id),mState(NORMAL),mTextW(0)
{
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

  if(opengl())
    setCaching(true);
}

void AGButton::setSurface(AGSurface pSurface,bool pChangeSize)
{
  mSurface=pSurface;
  AGImage *image=new AGImage(this,getRect().origin(),mSurface,false);
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


void AGButton::draw(AGPainter &p)
{
  p.pushMatrix();
  p.transform(AGRect2(0,0,width(),height()).shrink(borderWidth));
  mBG[mState].draw(p);
  mBorder[mState].draw(p);
  p.popMatrix();

  if(borderWidth==0)
    return;
  //  AGSurface s(getScreen());

  AGColor bc1;
  AGColor bc2;
  AGTheme *theme=getTheme();

  bc1=theme->getColor(mTheme+"button.border.color1");
  bc2=theme->getColor(mTheme+"button.border.color2");
  if(true)
  {
  
    //  CTRACE;

    //FIXME: use painter !!!!!!
    AGRect2 mr;//=r.project(getRect());
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
  queryRedraw();
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
  queryRedraw();
  if(!mEnabled)
    return false;
  if(mChecked)
    mState=CHECKED;
  else
    mState=NORMAL;
  return false;
}


bool AGButton::eventMouseButtonDown(AGEvent *e)
{
  queryRedraw();
  if(!mEnabled)
    return false;

  // CTRACE;
  if(e->isSDLEvent())
    {
      //      cdebug(getScreenRect());
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  if(mChecked)
	    mState=CHECKEDPRESSED;
	  else
	    mState=PRESSED;
	  //  ccdebug("Pressed");
	  //	  sigClick(m);
	  //	  cdebug(getName());
	  AGWidget::eventMouseButtonDown(e); // let it get the buttondown-info

	  return true;// FIXME:TEST false; // events only get eaten if in window
	}
    }
  return AGWidget::eventMouseButtonDown(e);
}

bool AGButton::eventMouseButtonUp(AGEvent *e)
{
  queryRedraw();
  if(!mEnabled)
    return false;

  if(e->isSDLEvent())
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
  return AGWidget::eventMouseButtonUp(e);
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
  AGButton b(0,AGRect2(0,0,0,0),"test");
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
  queryRedraw();
  mTheme=addPoint(pTheme);

  lower=getTheme()->getInt(mTheme+"buttonLowerOnClick");
  borderWidth=getTheme()->getInt(mTheme+"button.border.width");

  mBG[NORMAL]=AGBackground(mTheme+"button.background.normal");
  mBG[PRESSED]=AGBackground(mTheme+"button.background.pressed");
  mBG[LIGHTED]=AGBackground(mTheme+"button.background.lighted");
  mBG[CHECKED]=AGBackground(mTheme+"button.background.checked");
  mBG[CHECKEDLIGHTED]=AGBackground(mTheme+"button.background.checkedlighted");
  mBG[CHECKEDPRESSED]=AGBackground(mTheme+"button.background.checkedpressed");

  mBorder[NORMAL]=AGBorder(mTheme+"button.border.normal");
  mBorder[PRESSED]=AGBorder(mTheme+"button.border.pressed");
  mBorder[LIGHTED]=AGBorder(mTheme+"button.border.lighted");
  mBorder[CHECKED]=AGBorder(mTheme+"button.border.checked");
  mBorder[CHECKEDLIGHTED]=AGBorder(mTheme+"button.border.checkedlighted");
  mBorder[CHECKEDPRESSED]=AGBorder(mTheme+"button.border.checkedpressed");

  if(mTextW)
    mTextW->setTheme(mTheme+"button.text");

}

void AGButton::setCaption(const std::string &pCaption)
{
  queryRedraw();
  mText=pCaption;
  if(mTextW)
    mTextW->setText(pCaption);
}

void AGButton::setState(const State &pState)
{
  queryRedraw();
  mState=pState;
}

void AGButton::setChecked(bool pChecked)
{
  queryRedraw();
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

bool AGButton::canFocus() const
{
  return true;
}

void AGButton::useTextures()
{
  for(std::map<State,AGBackground>::iterator i=mBG.begin();i!=mBG.end();++i)
    i->second.useTextures();
  for(std::map<State,AGBorder>::iterator i=mBorder.begin();i!=mBorder.end();++i)
    i->second.useTextures();
  
}
