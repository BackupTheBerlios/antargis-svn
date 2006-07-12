/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_widget.cc
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

#include "ag_widget.h"
#include "ag_debug.h"
#include "ag_screen.h"
#include "ag_menu.h"
#include "ag_kill.h"
#include <map>
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <ag_tooltip.h>
#include "ag_application.h"
#include "ag_layout.h"
#include "ag_config.h"

#define FOCUS_BY_SORT


using namespace std;

AGWidget *agNoParent=0;

class MWidgetSet:public std::set<AGWidget*>
{
public:
  virtual ~MWidgetSet();
};
MWidgetSet *mPAllWidgets=0; // workaround - to check if still widget exists or not
bool mWidgetSetDeleted=false;

MWidgetSet::~MWidgetSet()
{
  CTRACE;
  mPAllWidgets=0;
  mWidgetSetDeleted=true;
}

MWidgetSet *getAllWidgets()
{
  if(!mPAllWidgets)
    {
      if(!mWidgetSetDeleted)
	{
	  mPAllWidgets=new MWidgetSet;
	  REGISTER_SINGLETON(mPAllWidgets);
	}
    }
  return mPAllWidgets;
}


AGWidget::AGWidget(AGWidget *pParent,const AGRect2 &r):
  sigMouseEnter(this,"sigMouseEnter"),
  sigMouseLeave(this,"sigMouseLeave"),
  sigClick(this,"sigClick"),
  mr(r),mParent(pParent),mChildrenEventFirst(false),mChildrenDrawFirst(false),mMouseIn(false),mButtonDown(false),
  mFixedWidth(false),mFixedHeight(false),mVisible(true),mCaching(false),
  mHasFocus(false),mFocus(0)

{
  mChangeRect=AGRect2(0,0,0,0);
  mCache=0;
  mCacheTouched=false;
  mTooltipWidget=0;
  mRubyObject=false;
  mModal=false;
  if(getAllWidgets())
    getAllWidgets()->insert(this);
}

AGWidget::~AGWidget()
{
  CTRACE;
  std::list<AGWidget*>::iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    {
      (*i)->setParent(0);
    }
  if(getParent())
    {
      if(getAllWidgets())
	{
	  if(getAllWidgets()->find(getParent())==getAllWidgets()->end())
	    {
	      cdebug("WARNING:Error in ~AGWidget!!!");
	    }
	  else
	    getParent()->eventChildrenDeleted(this);
	}
    }
  if(getAllWidgets())
    getAllWidgets()->erase(this);
}

void AGWidget::removeChild(AGWidget *w)
{
  std::list<AGWidget*>::iterator i=std::find(mChildren.begin(),mChildren.end(),w);
  if(i!=mChildren.end())
    {
      mChildren.erase(i);
      w->setParent(0);
    }
}


void AGWidget::eventChildrenDeleted(AGWidget *pWidget)
{
  std::list<AGWidget*>::iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    {
      if(*i==pWidget)
	{
	  mChildren.erase(i);
	  break;
	}
    }
  i=mToClear.begin();
  for(;i!=mToClear.end();i++)
    {
      if(*i==pWidget)
	{
	  mToClear.erase(i);
	  break;
	}
    }
}


void AGWidget::draw(AGPainter &p)
{
}

void AGWidget::delObjects()
{
 // clearing widgets
  if(mToClear.size())
    {
      std::list<AGWidget*>::iterator i=mToClear.begin();
      for(;i!=mToClear.end();i++)
	{
	  if(!(*i)->mRubyObject) // don't delete ruby-objects - they get deleted by garbage collection
	    {
	      cdebug("type:"<<typeid(**i).name());
	      delete *i;
	    }
	}
      mToClear.clear();
    }
}

void AGWidget::drawAll(AGPainter &p)
{
  delObjects();

  if(!mVisible)
    return;

  if(mCache)
    {
      p.blit(*mCache,getRect(),getRect().origin());
    }
  else
    {
      p.pushMatrix();

      p.transform(getRect());

      if(!mChildrenDrawFirst)
	draw(p);

      std::list<AGWidget*>::reverse_iterator i=mChildren.rbegin(); // draw from back to front
      
      for(;i!=mChildren.rend();i++)
	(*i)->drawAll(p);
      
      if(mChildrenDrawFirst)
	draw(p);

      drawAfter(p);
    
      p.popMatrix();
      setDrawn();
      //	  getScreen().flip();
      //	  SDL_Delay(500);
    }
}

AGRect2 AGWidget::getRect() const
{
  return mr;
}

AGRect2 AGWidget::getClientRect() const
{
  return mr.origin();
}

bool AGWidget::processEvent(AGEvent *event)
{
  if(!mVisible)
    return false;

  bool processed = false;
  // do i have a capturehook set ? (modal)
  // i will send that event to my children

  std::list<AGWidget*>::iterator i;

  std::list<AGWidget*> children=mChildren; // copy children, so that changes do not affect iteration
  for(i=children.begin();i!=children.end() && !processed; i++)
    processed=(*i)->processEvent(event);
  
  if(processed)
    return processed;

  event->setCaller(this);
  
  // let me see if i can process it myself
  if(AGMessageObject::processEvent(event))
    return true;
  
  checkFocus();
  if(mModal)
    return true;
  return false;
}

bool AGWidget::eventShow()
{
  return false;
}
bool AGWidget::eventHide()
{
  return false;
}

bool AGWidget::eventMouseEnter()
{
  if(mTooltip.length())
    {
      mTooltipWidget=new AGTooltip(getScreenRect(),mTooltip);
      getApplication()->setTooltip(mTooltipWidget);
    }

  return false;
}
bool AGWidget::eventMouseLeave()
{
  if(mTooltipWidget)
    {
      getApplication()->resetTooltip(mTooltipWidget);
      mTooltipWidget=0;
    }
  return false;
}

bool AGWidget::eventMouseMotion(AGEvent *e)
{
  if(e->isSDLEvent())
    {
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  if(!mMouseIn)
	    {
	      mMouseIn=true;
	      eventMouseEnter() || sigMouseEnter(e);
	    }
	}
      else
	{
	  if(mMouseIn)
	    {
	      mMouseIn=false;
	      eventMouseLeave() || sigMouseLeave(e);
	    }
	}

      if(mButtonDown)
	{
	  //	  TRACE;
	  AGVector2 v=e->getMousePosition()-mOldMousePos;
	  e->setVector(v);
	  eventDragBy(e,v) || sigDragBy(e);
	  mOldMousePos=e->getMousePosition();
	}
    }
  return false;
}

bool AGWidget::eventMouseButtonDown(AGEvent *e)
{
  if(e->isSDLEvent())
    {
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  mButtonDown=true;
	  mOldMousePos=e->getMousePosition();
	}
    }
  return false;
}

bool AGWidget::eventMouseButtonUp(AGEvent *e)
{
  bool was=mButtonDown;

  mButtonDown=false;
  
  if(e->isSDLEvent())
    {
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  if(was)
	    {
	      e->setName("sigClick");
	      if(!isParent(getApplication()->getOverlay()))
		getApplication()->setOverlay(0);
	      
	      if(canFocus())
		gainFocus();

	      return (eventMouseClick(e) || sigClick(e));

	    }
	}
    }
  return false;

}

bool AGWidget::eventMouseClick(AGEvent *m)
{
  return false;
}

void AGWidget::addChild(AGWidget *w)
{
  mChildren.push_front(w); // set on top
  if(mHasFocus && w->canFocus())
    {
      gainFocus(w);
    }
  queryRedraw();
}

void AGWidget::clear()
{
  // delay it till be draw everything - so this doesn't kill widgets while processing events
  std::copy(mChildren.begin(),mChildren.end(),std::back_inserter(mToClear));
  mChildren.clear();
}

void AGWidget::erase(AGWidget *w)
{
  std::list<AGWidget*>::iterator i=std::find(mChildren.begin(),mChildren.end(),w);
  if(i!=mChildren.end())
    {
      mToClear.push_back(w);
      
      mChildren.erase(i);
    }
}

void AGWidget::addChildBack(AGWidget *w)
{
  mChildren.push_back(w); // set on top
}

void AGWidget::regChange()
{
  AGRect2 t=getScreenRect().grow(20);
  if(mChangeRect.width()==0 || mChangeRect.height()==0)
    mChangeRect=t;
  else
    {
      mChangeRect.include(t[0]);
      mChangeRect.include(t[1]);
    }
}

void AGWidget::setRect(const AGRect2 &pRect)
{
  regChange();
  mr=pRect;
  regChange();
  if(mParent)
    mParent->redraw();
}

float AGWidget::minWidth() const
{
  // accumulate max
  std::list<AGWidget*>::const_iterator i=mChildren.begin();
  float m=0;

  for(;i!=mChildren.end();i++)
    m=std::max(m,(*i)->minWidth());

  return m;
}

float AGWidget::minHeight() const
{
  // accumulate max
  std::list<AGWidget*>::const_iterator i=mChildren.begin();
  float m=0;

  for(;i!=mChildren.end();i++)
    m=std::max(m,(*i)->minHeight());

  return m;
}

float AGWidget::width() const
{
  return mr.w();
}

float AGWidget::height() const
{
  return mr.h();
}

bool AGWidget::fixedWidth() const
{
  return mFixedWidth;
}

bool AGWidget::fixedHeight() const
{
  return mFixedHeight;
}

void AGWidget::setWidth(float w)
{
  regChange();
  mr.setWidth(w);
  regChange();
}
void AGWidget::setHeight(float h)
{
  regChange();
  mr.setHeight(h);
  regChange();
}

void AGWidget::setTop(float y)
{
  regChange();
  mr.setTop(y);
  regChange();
}
void AGWidget::setLeft(float x)
{
  regChange();
  mr.setLeft(x);
  regChange();
}
float AGWidget::bottom() const
{
  return mr[1][1];
}
float AGWidget::right() const
{
  return mr[1][0];
}


float AGWidget::top() const
{
  return mr.y();
}

float AGWidget::left() const
{
  return mr.x();
}

void AGWidget::show()
{
  mVisible=true;
  queryRedraw();
}
void AGWidget::hide()
{
  mVisible=false;
  queryRedraw();
}

void AGWidget::setVisible(bool v)
{
  if(mVisible!=v)
    queryRedraw();
  mVisible=v;
}


void AGWidget::setParent(AGWidget *pParent)
{
  mParent=pParent;
}


AGWidget *AGWidget::getParent()
{
  return mParent;
}

AGVector2 AGWidget::getScreenPosition() const
{
  AGVector2 p=mr[0];

  if(mParent)
    p+=mParent->getScreenPosition();

  return p;
}


AGRect2 AGWidget::getScreenRect() const
{
  AGRect2 srect(mr.origin()+getScreenPosition());
  return srect;
}

AGVector2 AGWidget::fromScreen(const AGVector2 &p) const
{
  if(!mParent)
    return p;
  AGRect2 r(mParent->getScreenRect());
  return p-r[0];
}

bool AGWidget::canFocus() const
{
  std::list<AGWidget*>::const_iterator i=mChildren.begin();

  for(;i!=mChildren.end();i++)
    if((*i)->canFocus())
      {

	return true;
      }

  return false;
}

bool AGWidget::eventGotFocus()
{
  mHasFocus=true;
  return false;
}

bool AGWidget::eventLostFocus()
{
  if(mFocus)
    mFocus->eventLostFocus();
  mHasFocus=false;
  mFocus=0;
  
  return false;
}
/*
void AGWidget::setMenu(AGMenu *pMenu)
{
  mMenu=pMenu;
  }*/

void AGWidget::gainCompleteFocus(AGWidget *pWidget)
{
#ifdef FOCUS_BY_SORT
  if(mParent)
    mParent->gainCompleteFocus(this);
  if(pWidget)
    {
      //      cdebug(mChildren.size());
      std::list<AGWidget*>::iterator i=std::find(mChildren.begin(),mChildren.end(),pWidget);
      if(i!=mChildren.end())
	{
	  mChildren.erase(i);
	  mChildren.push_front(pWidget);
	}
      //      cdebug(mChildren.size());
    }
#endif
}

void AGWidget::gainFocus(AGWidget *pWidget)
{
#ifdef FOCUS_BY_SORT
  if(pWidget)
    {
      //      cdebug(mChildren.size());
      std::list<AGWidget*>::iterator i=std::find(mChildren.begin(),mChildren.end(),pWidget);
      if(i!=mChildren.end())
	{
	  mChildren.erase(i);

	  if(mChildren.size()>0)
	    (*mChildren.begin())->eventLostFocus();

	  mChildren.push_front(pWidget);
	  pWidget->eventGotFocus();

	}
      //      cdebug(mChildren.size());
    }
  else if(mParent)
    {
      if(canFocus())
	{
	  mParent->gainFocus(this);
	}
    }
#else
  if(pWidget==0 && mParent)
    mParent->gainFocus(this);
  else if(mParent)
    mParent->gainFocus(pWidget);
  else
    gainFocusDown(pWidget);
#endif
}

void AGWidget::gainFocusDown(AGWidget *pWidget)
{
  //  CTRACE;
  std::list<AGWidget*>::iterator i;
  i=std::find(mChildren.begin(),mChildren.end(),pWidget);
  if(i!=mChildren.end())
    {
      // found
      if(!mHasFocus)
	{
	  if(mParent)
	    mParent->gainFocus(this);
	  else
	    {
	      mHasFocus=true;
	      eventGotFocus();
	    }
	}
      
      if(mFocus!=pWidget)
	{
	  if(mFocus)
	    mFocus->eventLostFocus();
	  mFocus=pWidget;
	  mFocus->eventGotFocus();
	}
    }
  else
    {
      for(i=mChildren.begin();i!=mChildren.end();i++)
	(*i)->gainFocusDown(pWidget);
    }
}

void AGWidget::checkFocus()
{
  if(mChildren.size()>0 && mFocus && mHasFocus)
    {
      if(mFocus!=*mChildren.begin())
	{
	  //	  TRACE;
	  std::list<AGWidget*>::iterator i;
	  
	  i=std::find(mChildren.begin(),mChildren.end(),mFocus);
	  // delete children and set to front 
	  mChildren.erase(i);
	  mChildren.push_front(mFocus);
	  //	  cdebug("mchildren #:"<<mChildren.size());
	}
    }
}

bool AGWidget::hasFocus(const AGWidget *pWidget)
{
#ifdef FOCUS_BY_SORT
  if(pWidget==0)
    {
      if(mParent)
	return mParent->hasFocus(this);
      else
	return true;
    }
  if(mChildren.size()==0)
    return true; // some error

  if(*mChildren.begin()==pWidget)
    {
      if(mParent)
	return mParent->hasFocus(this); // ok - so go on and check if "this" has focus
      return true; // ok
    }

  return false;
#else
  if(pWidget==0)
    {
      if(mParent)
	return mParent->hasFocus(this);
      else
	return true;
    }
  else if(mFocus!=pWidget)
    return false;
  else if(mParent)
    return mParent->hasFocus(this);
  return true;
#endif
}


bool AGWidget::eventDragBy(AGEvent *event,const AGVector2 &pDiff)
{
  return false;
}

bool AGWidget::getFocus() const
{
  return mHasFocus;
}

std::string AGWidget::getName() const
{
  return mName;
}
void AGWidget::setName(const std::string &pName)
{
  mName=pName;
}

void AGWidget::drawAfter(AGPainter &p)
{
}

bool AGWidget::visible() const
{
  return mVisible;
}


AGWidget *AGWidget::getChild(const std::string &pName)
{
  if(mName==pName)
    return this;

  AGWidget *w=0;
  std::list<AGWidget*>::iterator i=mChildren.begin();

  for(;i!=mChildren.end();i++)
    {
      w=(*i)->getChild(pName);
      if(w)
	break;
    }
  return w;
}

void AGWidget::setModal(bool pModal)
{
  mModal=pModal;
}

void AGWidget::mark()
{
  for(std::list<AGWidget*>::iterator i=mChildren.begin();i!=mChildren.end();i++)
    {
      markObject(*i);
    }

  // mark mToClear also - as it can be that they are still used on stack
  for(std::list<AGWidget*>::iterator i=mToClear.begin();i!=mToClear.end();i++)
    markObject(*i);
}



AGWidget *toAGWidget(AGMessageObject *o)
{
  return dynamic_cast<AGWidget*>(o);
}


bool AGWidget::redraw() const
{
  return mCacheTouched;
}

bool AGWidget::checkRedraw() const
{
  if(redraw())
    return true;
  for(std::list<AGWidget*>::const_iterator i=mChildren.begin();i!=mChildren.end();++i)
    if((*i)->checkRedraw())
      return true;
  return false;
}


void AGWidget::prepareDraw()
{
  if(!mVisible)
    return;

  if(mCache)
    {
      if(checkRedraw() || !mCache->hasTexture())
	{
	  for(std::list<AGWidget*>::iterator i=mChildren.begin();i!=mChildren.end();++i)
	    (*i)->prepareDraw();
	  mCache->clearContent();

	  AGPainter p(*mCache);

	  if(!mChildrenDrawFirst)
	    draw(p);
	  
	  std::list<AGWidget*>::reverse_iterator i=mChildren.rbegin(); // draw from back to front
	  
	  for(;i!=mChildren.rend();i++)
	    (*i)->drawAll(p);

	  if(mChildrenDrawFirst)
	    draw(p);

	  drawAfter(p);
	  mCacheTouched=false;
	  setDrawn();

	  if(mParent)
	    mParent->queryRedraw();
	}
    }
  else
    {
      for(std::list<AGWidget*>::iterator i=mChildren.begin();i!=mChildren.end();++i)
	(*i)->prepareDraw();
    }
}
void AGWidget::setCaching(bool pEnable)
{
  if(getConfig()->get("widgetTextureCache")=="false")
    return;
  getConfig()->set("widgetTextureCache","true");

  mCaching=pEnable;
  delete mCache;

  mCache=0;
  mCacheTouched=false;
  if(pEnable)
    {
      mCache=new AGTexture(width(),height());
      
      mCacheTouched=true;
    }
}

void AGWidget::setDrawn()
{
  mCacheTouched=false;
  for(std::list<AGWidget*>::iterator i=mChildren.begin();i!=mChildren.end();++i)
    (*i)->setDrawn();
  mChangeRect=AGRect2(0,0,0,0);
}

void AGWidget::queryRedraw()
{
  mCacheTouched=true;
  regChange();
}

void AGWidget::useTextures()
{
}

void AGWidget::useTexturesRecursive()
{
  useTextures();
  for(std::list<AGWidget*>::iterator i=mChildren.begin();i!=mChildren.end();++i)
    (*i)->useTexturesRecursive();
}

AGRect2 AGWidget::getChangeRect()
{
  AGRect2 r=mChangeRect;
  
  for(std::list<AGWidget*>::iterator i=mChildren.begin();i!=mChildren.end();++i)
    {
      AGRect2 n=(*i)->getChangeRect();
      if(n.width()!=0 && n.height()!=0)
	{
	  if(r.width()!=0 && r.height()!=0)
	    {
	      r.include(n[0]);
	      r.include(n[1]);
	    }
	  else
	    r=n;
	}
    }
  return r;
}

void AGWidget::setTooltip(const std::string &pTooltip)
{
  mTooltip=pTooltip;
}

AGLayout *AGWidget::getLayout()
{
  AGLayout *l=dynamic_cast<AGLayout*>(this);
  if(l)
    return l;
  if(mParent)
    return mParent->getLayout();
  return 0;
}

void AGWidget::initHandlers()
{
}

void AGWidget::eventTick(float pTime)
{
}
void AGWidget::sigTick(float pTime)
{
  eventTick(pTime);
  for(std::list<AGWidget*>::iterator i=mChildren.begin();i!=mChildren.end();++i)
    (*i)->sigTick(pTime);
}

void AGWidget::close()
{
  if(mParent)
    mParent->removeChild(this);
}

bool AGWidget::isParent(AGWidget *pParent)
{
  if(mParent==pParent)
    return true;
  else if(mParent!=0)
    return mParent->isParent(pParent);
  return false;
}
