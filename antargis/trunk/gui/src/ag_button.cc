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
  mImageW=0;
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
  if(!mImageW)
    {
      mImageW=new AGImage(this,getRect().origin(),mSurface,false);
      addChild(mImageW);
    }
  else
    {
      mImageW->setSurface(pSurface);
      mImageW->show();
    }
  mTextW->hide();
  if(pChangeSize)
    {
      setWidth(mImageW->width()+2+2*borderWidth);
      setHeight(mImageW->height()+2+2*borderWidth);
    }

  else
    {
      // then center
      mImageW->setLeft((width()-mImageW->width())/2);
      mImageW->setTop((height()-mImageW->height())/2);
    }
  queryRedraw();
}


void AGButton::draw(AGPainter &p)
{
  p.pushMatrix();
  p.transform(AGRect2(0,0,width(),height()).shrink(borderWidth));
  AGRect2 pr=getRect().origin();
  mBG[mState].draw(pr,p);
  mBorder[mState].draw(pr,p);
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
  AGWidget::eventMouseEnter();
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
  AGWidget::eventMouseLeave();
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
  if(e->isSDLEvent())
    {
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  if(mEnabled)
	    {
	      queryRedraw();
	      if(mChecked)
		mState=CHECKEDPRESSED;
	      else
		mState=PRESSED;

	      AGWidget::eventMouseButtonDown(e); // let it get the buttondown-info
	    }
	  
	  return true;
	}
    }
  return AGWidget::eventMouseButtonDown(e);
}

bool AGButton::eventMouseButtonUp(AGEvent *e)
{
  if(e->isSDLEvent())
    {
      if(mEnabled)
	{
	  queryRedraw();
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
     
    }
  return AGWidget::eventMouseButtonUp(e);
}

void AGButton::setWidth(float w)
{
  assert(w>=0);
  ccdebug(w);
  AGWidget::setWidth(w);
  std::list<AGWidget*>::iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    (*i)->setRect(getRect().shrink(borderWidth));
}
void AGButton::setHeight(float h)
{
  assert(h>=0);
  AGWidget::setHeight(h);
  std::list<AGWidget*>::iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    (*i)->setRect(getRect().shrink(borderWidth));
}

std::string AGButton::getCaption() const
{
  return mText;
}

void AGButton::setEnabled(bool pEnable)
{
  if(mEnabled!=pEnable)
    {
      mEnabled=pEnable;
      setState(mState); // check
      queryRedraw();
    }
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
  mBG[DISABLED]=AGBackground(mTheme+"button.background.disabled");

  mBorder[NORMAL]=AGBorder(mTheme+"button.border.normal");
  mBorder[PRESSED]=AGBorder(mTheme+"button.border.pressed");
  mBorder[LIGHTED]=AGBorder(mTheme+"button.border.lighted");
  mBorder[CHECKED]=AGBorder(mTheme+"button.border.checked");
  mBorder[CHECKEDLIGHTED]=AGBorder(mTheme+"button.border.checkedlighted");
  mBorder[CHECKEDPRESSED]=AGBorder(mTheme+"button.border.checkedpressed");
  mBorder[DISABLED]=AGBorder(mTheme+"button.border.disabled");

  if(mTextW)
    mTextW->setTheme(mTheme+"button.text");

}

void AGButton::setCaption(const std::string &pCaption)
{
  queryRedraw();
  mText=pCaption;
  if(mTextW)
    mTextW->setText(pCaption);
  if(mImageW)
    mImageW->hide();
}

void AGButton::setState(const State &pState)
{
  queryRedraw();
  if(mEnabled)
    {
      if(mState==DISABLED)
	mState=NORMAL;
      else
	mState=pState;
    }
  else
    mState=DISABLED;
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
