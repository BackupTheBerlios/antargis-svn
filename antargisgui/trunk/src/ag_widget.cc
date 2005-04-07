#include "ag_widget.h"
#include "ag_debug.h"
#include "ag_menu.h"
#include <map>
#include <iostream>

using namespace std;

AGWidget *agNoParent=0;


AGWidget::AGWidget(AGWidget *pParent,const AGRect &r):
  sigMouseEnter(this,"sigMouseEnter"),
  sigMouseLeave(this,"sigMouseLeave"),
  sigClick(this,"sigClick"),
  mr(r),mParent(pParent),mChildrenEventFirst(false),mChildrenDrawFirst(false),mMouseIn(false),mButtonDown(false),
  mFixedWidth(false),mFixedHeight(false),mVisible(true),mMenu(0),hasFocus(false),mFocus(0)

{
  /*  if(pParent)
      pParent->addChild(this);*/
}
AGWidget::~AGWidget()
{
}

void AGWidget::draw(const AGRect &r)
{
}

void AGWidget::drawAll(const AGRect &r)
{
  //  TRACE;
  if(!mVisible)
    return;
cdebug(0);
cdebug(this);
cdebug(mChildrenDrawFirst);
cdebug(typeid(*this).name());
  if(!mChildrenDrawFirst)
    draw(r);
cdebug(1);
  std::list<AGWidget*>::reverse_iterator i=mChildren.rbegin(); // draw from back to front
  AGRect r2=r.project(mr);
cdebug(2);
  for(;i!=mChildren.rend();i++)
    (*i)->drawAll(r2);
cdebug(3);
  if(mChildrenDrawFirst)
    draw(r);

cdebug(4);
  if(mMenu)
    mMenu->drawAll(r2);
cdebug(5);

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
	    }
	}
    }
  return false;

}

bool AGWidget::eventMouseClick(const AGEvent *m)
{
  CTRACE;
  return false;
}

void AGWidget::addChild(AGWidget *w)
{
  mChildren.push_front(w); // set on top
  if(hasFocus && w->canFocus())
    {
      gainFocus(w);
    }
}
void AGWidget::addChildBack(AGWidget *w)
{
  mChildren.push_back(w); // set on top
}

void AGWidget::setRect(const AGRect &pRect)
{
  mr=pRect;
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

AGRect AGWidget::getScreenRect() const
{
  AGRect srect(getRect());
  if(mParent)
    srect=mParent->getScreenRect().project(srect);
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
      return true;

  return false;
}

bool AGWidget::eventGotFocus()
{
  //  CTRACE;
  hasFocus=true;
  return false;
}

bool AGWidget::eventLostFocus()
{
  //  CTRACE;
  if(mFocus)
    mFocus->eventLostFocus();
  hasFocus=false;
  mFocus=0;
  
  return false;
}

void AGWidget::setMenu(AGMenu *pMenu)
{
  mMenu=pMenu;
}

void AGWidget::gainFocus(AGWidget *pWidget)
{
  //  CTRACE;
  Assert(std::find(mChildren.begin(),mChildren.end(),pWidget)!=mChildren.end());

  if(!hasFocus)
    {
      if(mParent)
	mParent->gainFocus(this);
      else
	{
	  hasFocus=true;
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

void AGWidget::checkFocus()
{
  if(mChildren.size()>0 && mFocus && hasFocus)
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


bool AGWidget::eventDragBy(const AGEvent *event,const AGPoint &pDiff)
{
  //  TRACE;
  return false;
}

bool AGWidget::getFocus() const
{
  //  CTRACE;
  //  cdebug(hasFocus);
  return hasFocus;
}



#ifdef USE_RUBY
#include "ruby.h"
#undef connect
#endif
void AGWidget_markfunc(void *ptr)
{
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
      rubyAnimal = cppAnimal->mRUBY;//SWIG_RubyInstanceFor(cppAnimal);
      rb_gc_mark(rubyAnimal);
    }
#endif
}


