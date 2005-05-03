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
#include "ag_menu.h"
#include <map>
#include <iostream>
#include <algorithm>

#define FOCUS_BY_SORT


using namespace std;

AGWidget *agNoParent=0;


AGWidget::AGWidget(AGWidget *pParent,const AGRect &r):
  sigMouseEnter(this,"sigMouseEnter"),
  sigMouseLeave(this,"sigMouseLeave"),
  sigClick(this,"sigClick"),
  mr(r),mParent(pParent),mChildrenEventFirst(false),mChildrenDrawFirst(false),mMouseIn(false),mButtonDown(false),
  mFixedWidth(false),mFixedHeight(false),mVisible(true),mMenu(0),mHasFocus(false),mFocus(0)

{
  mRubyObject=false;
  mModal=false;
  //  cdebug(r);
  /*  if(pParent)
      pParent->addChild(this);*/
}
AGWidget::~AGWidget()
{
  CTRACE;
  cdebug(typeid(*this).name());
  cdebug("Name:"<<getName());
  cdebug(getName());
  //  throw int();
}

void AGWidget::draw(AGPainter &p)
{
}

void AGWidget::drawAll(AGPainter &p)
{
  //  CTRACE;
  //  cdebug(getRect());
  AGPainter p2(p);
  p2.transform(getRect());
  //  cdebug(p2.getRect());

  if(mToClear.size())
    {
      //      cdebug("mClear:"<<mToClear.size());
      //      CTRACE;
      //      cdebug(mChildren.size());
      std::list<AGWidget*>::iterator i=mToClear.begin();
      for(;i!=mToClear.end();i++)
	{
	  if(!(*i)->mRubyObject) // don't delete ruby-objects - they get deleted by garbage collection
	    delete *i;
	}
      mToClear.clear();
    }



  //  TRACE;
  if(!mVisible)
    return;

  if(!mChildrenDrawFirst)
    {
      //      cdebug(p2.getRect());
      draw(p2);
    }





  std::list<AGWidget*>::reverse_iterator i=mChildren.rbegin(); // draw from back to front
  //  AGRect r2=r.project(mr);
  for(;i!=mChildren.rend();i++)
    (*i)->drawAll(p2);
  if(mChildrenDrawFirst)
    draw(p2);

  if(mMenu)
    mMenu->drawAll(p2);
}

AGRect AGWidget::getRect() const
{
  return mr;
}

AGRect AGWidget::getClientRect() const
{
  return AGRect(0,0,mr.w,mr.h);
}

bool AGWidget::processEvent(const AGEvent *event)
{
  if(!mVisible)
    return false;
  //  cdebug(typeid(this));

  //  TRACE;
  //const SDL_Event * event, bool bModal) {
  
  bool processed = false;
  // do i have a capturehook set ? (modal)
  // i will send that event to my children

  std::list<AGWidget*>::iterator i=mChildren.begin();

  if(mMenu)
    processed=mMenu->processEvent(event);

  for(;i!=mChildren.end() && !processed; i++)
    processed=(*i)->processEvent(event);
  
  if(processed) {
    //    cdebug("processed by child"<<endl);
    return processed;
  }

  
  // let me see if i can process it myself
  
  if(AGMessageObject::processEvent(event)) {
    

    //    cdebug("processed by me"<<endl);
    return true;
  }
  /*  
  if(bModal) {
    return processed;
  }
  
  // ask my parent to process the event
  
  if(GetParent()) {
    if(GetParent()->ProcessEvent(event)) {
      return true;
    }
  }
  */
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
  return false;
}
bool AGWidget::eventMouseLeave()
{
  return false;
}

bool AGWidget::eventMouseMotion(const AGEvent *m)
{
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
    {
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  if(!mMouseIn)
	    {
	      mMouseIn=true;
	      eventMouseEnter();
	    }
	}
      else
	{
	  if(mMouseIn)
	    {
	      mMouseIn=false;
	      eventMouseLeave();
	    }
	}

      if(mButtonDown)
	{
	  //	  TRACE;
	  eventDragBy(m,e->getMousePosition()-mOldMousePos);
	  mOldMousePos=e->getMousePosition();
	}
    }
  return false;
}

bool AGWidget::eventMouseButtonDown(const AGEvent *m)
{
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
    {
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  mButtonDown=true;
	  mOldMousePos=e->getMousePosition();
	  //return true; // consume

	  /*
	  cdebug(int(e->getButton()));
	  cdebug((void*)mMenu);
	  cdebug(int(e->getButton())<<" "<<int(SDL_BUTTON_RIGHT));*/
	  if((e->getButton()==SDL_BUTTON_RIGHT) && mMenu)
	    mMenu->show(e->getMousePosition());//getScreenRect().getPosition());
	}
    }
  return false;
}

bool AGWidget::eventMouseButtonUp(const AGEvent *m)
{
  //  cdebug("typeid:"<<typeid(*this).name());
  bool was=mButtonDown;
  mButtonDown=false;
  
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
    {
      if(getScreenRect().contains(e->getMousePosition()))
	{
	  if(was)
	    {
  
	      //	      cdebug("click");
	      eventMouseClick(m) || sigClick(m);
	      //  return true; // consume

	      if(canFocus())
		gainFocus();
	    }
	}
    }
  return false;

}

bool AGWidget::eventMouseClick(const AGEvent *m)
{
  //  CTRACE;
  return false;
}

void AGWidget::addChild(AGWidget *w)
{
  mChildren.push_front(w); // set on top
  if(mHasFocus && w->canFocus())
    {
      gainFocus(w);
    }
#ifdef USE_RUBY
  if(w->mRubyObject)
    {
      VALUE rubyAnimal = w->mRUBY;
      rb_gc_mark(rubyAnimal);
      cdebug("mark:"<<w->getName());
   }

#endif
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

void AGWidget::setRect(const AGRect &pRect)
{
  mr=pRect;
  cdebug(mr);
}

int AGWidget::minWidth() const
{
  // accumulate max
  std::list<AGWidget*>::const_iterator i=mChildren.begin();
  int m=0;

  for(;i!=mChildren.end();i++)
    m=std::max(m,(*i)->minWidth());

  return m;
}

int AGWidget::minHeight() const
{
  // accumulate max
  std::list<AGWidget*>::const_iterator i=mChildren.begin();
  int m=0;

  for(;i!=mChildren.end();i++)
    m=std::max(m,(*i)->minHeight());

  return m;
}

int AGWidget::width() const
{
  return mr.w;
}

int AGWidget::height() const
{
  return mr.h;
}

bool AGWidget::fixedWidth() const
{
  return mFixedWidth;
}

bool AGWidget::fixedHeight() const
{
  return mFixedHeight;
}

void AGWidget::setWidth(int w)
{
  mr.w=w;
}
void AGWidget::setHeight(int h)
{
  //  cdebug(h);
  mr.h=h;
}

void AGWidget::setTop(int y)
{
  //  int d=y-mr.y;
  mr.y=y;
  /*
  std::list<AGWidget*>::iterator i=mChildren.begin();

    for(;i!=mChildren.end();i++)
    {
      (*i)->setTop((*i)->top()+d);
    }
  */
}
void AGWidget::setLeft(int x)
{
  //  int d=x-mr.x;
  mr.x=x;
  /*
  std::list<AGWidget*>::iterator i=mChildren.begin();

  for(;i!=mChildren.end();i++)
    {
      (*i)->setLeft((*i)->left()+d);
    }
  */
}
int AGWidget::top() const
{
  return mr.y;
}

int AGWidget::left() const
{
  return mr.x;
}

void AGWidget::show()
{
  mVisible=true;
}
void AGWidget::hide()
{
  mVisible=false;
}

AGWidget *AGWidget::getParent()
{
  return mParent;
}

AGPoint AGWidget::getScreenPosition() const
{
  AGPoint p=mr.getPosition();
  //  cdebug(typeid(*this).name()<<" RECT:"<<getRect());
  if(mParent)
    p+=mParent->getScreenPosition();
  //  cdebug(typeid(*this).name());
  //  cdebug(p);
  return p;
}


AGRect AGWidget::getScreenRect() const
{
  AGRect srect(AGRect(0,0,mr.w,mr.h)+getScreenPosition());
  return srect;
}

AGPoint AGWidget::fromScreen(const AGPoint &p) const
{
  if(!mParent)
    return p;
  AGRect r(mParent->getScreenRect());
  return AGPoint(p.x-r.x,p.y-r.y);
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
  //  CTRACE;
  mHasFocus=true;
  return false;
}

bool AGWidget::eventLostFocus()
{
  //  CTRACE;
  if(mFocus)
    mFocus->eventLostFocus();
  mHasFocus=false;
  mFocus=0;
  
  return false;
}

void AGWidget::setMenu(AGMenu *pMenu)
{
  mMenu=pMenu;
}

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
	  mChildren.push_front(pWidget);
	}
      //      cdebug(mChildren.size());
    }
  else if(mParent)
    {
      if(canFocus())
	mParent->gainFocus(this);
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
  /*
  cdebug("firstchild:"<<(*mChildren.begin())->getName());
  cdebug("fc:"<<typeid(**mChildren.begin()).name());
  cdebug("me:"<<getName());
  cdebug("me:"<<typeid(*this).name());*/
  if(*mChildren.begin()==pWidget)
    {
      if(mParent)
	return mParent->hasFocus(this); // ok - so go on and check if "this" has focus
      return true; // ok
    }
  //  cdebug("no");

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


bool AGWidget::eventDragBy(const AGEvent *event,const AGPoint &pDiff)
{
  //  TRACE;
  return false;
}

bool AGWidget::getFocus() const
{
  //  CTRACE;
  //  cdebug(hasFocus);
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

#ifdef USE_RUBY
#include "ruby.h"
#undef connect
#endif
void AGWidget_markfunc(void *ptr)
{
  cdebug("TRACE");
#ifdef USE_RUBY
  AGWidget *cppAnimal;
  VALUE   rubyAnimal;
  AGWidget *zoo;
  
  TRACE;  
  cdebug(ptr<<endl);
  zoo = static_cast<AGWidget*>(ptr);
  
  std::list<AGWidget*>::iterator i=zoo->mChildren.begin();

  for(;i!=zoo->mChildren.end();i++)
    {
      cdebug("children:"<<*i);
      cppAnimal = *i;//zoo->getAnimal(i);
      if(cppAnimal->mRubyObject)
	{
	  rubyAnimal = cppAnimal->mRUBY;//SWIG_RubyInstanceFor(cppAnimal);
	  rb_gc_mark(rubyAnimal);
	  cdebug("mark:"<<cppAnimal->getName());
	}
      AGWidget_markfunc(*i);
    }
#endif
}



AGWidget *toAGWidget(AGMessageObject *o)
{
  return dynamic_cast<AGWidget*>(o);
}
