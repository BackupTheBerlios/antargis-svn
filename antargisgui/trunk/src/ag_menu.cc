#include "ag_menu.h"
#include "ag_menuitem.h"
#include "ag_font.h"
#include "ag_text.h"
#include "ag_surface.h"
#include "ag_theme.h"

#include "ag_debug.h"

using namespace std;

AGMenu::AGMenu(AGWidget *pParent,AGPoint pWishPos,const std::string &pName):
  AGVTiler(pParent,AGRect(pWishPos.x,pWishPos.y,1,1)),
  mName(pName),
  mWishPos(pWishPos)
{
  mBg=AGTexture(getTheme()->getSurface("backgroundSurface.menu"));
  hide();
}

void AGMenu::show(AGPoint pWishPos)
{
  AGWidget::show();
  mWishPos=pWishPos;
  AGPoint p=fromScreen(mWishPos);
  setTop(p.y);
  setLeft(p.x);
  rePosition();

  std::list<AGWidget*>::iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    cout<<(*i)->getRect().toString()<<endl;

  AGVTiler::show();
}

void AGMenu::addItem(const std::string &pString)
{
  //  AGFont font("Arial.ttf");
  //  AGText *b=new AGText(this,AGPoint(0,0),pString,font);
  addChild(new AGMenuItem(this,pString));//b);
}

AGMenu &AGMenu::addMenu(const std::string &pString)
{
  AGSubMenu *s;
  addChild(s=new AGSubMenu(this,pString));
  return *s->getMenu();
}


void AGMenu::draw(const AGRect &r)
{
  //  cdebug("draw");
  //  return;
  adaptWidthFromChildren();
  getScreen().tile(mBg,r.project(getRect()));

  //  cdebug(r.project(getRect()).toString());
}

void AGMenu::eventItemSelected(const std::string &pString)
{
  std::list<AGWidget*>::iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    {
      AGMenuItem *item=dynamic_cast<AGMenuItem*>(*i);
      //cdebug("item:"<<item);
      if(item)
	{
	  //  cdebug(item->getName()<<" "<<pString);
	  if(item->getSelected() && item->getName()!=pString)
	    {
	      //  cdebug("unSelect");
	      item->unSelect();
	    }
	}
    }
}
void AGMenu::eventItemClicked(const std::string &pString)
{
  sigSelected(new AGMenuEvent(this,pString));
  
  AGMenuItem *mi=dynamic_cast<AGMenuItem*>(getParent());
  if(mi)
    {
      AGMenu *m=dynamic_cast<AGMenu*>(mi->getParent());
      if(m)
	m->eventItemClicked(pString);
    }
  hide();
  cdebug("hide");
}

AGMenuEvent::AGMenuEvent(AGListener *pListener,const std::string &pName):
  AGEvent(pListener),mName(pName)
{
}

std::string AGMenuEvent::getName() const
{
  return mName;
}
