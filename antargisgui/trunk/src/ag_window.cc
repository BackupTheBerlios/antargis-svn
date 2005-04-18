/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_window.cc
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

#include "ag_window.h"
#include "ag_surface.h"
#include "ag_theme.h"
#include "ag_image.h"
#include "ag_text.h"
#include "ag_debug.h"
#include "ag_button.h"
#include "ag_caption.h"

#undef connect

AGWindow::AGWindow(AGWidget *pWidget,const AGRect &pRect,const std::string &pTitle):
  AGTable(pWidget,pRect),mTitle(pTitle)
{
  //  CTRACE;
  AGSurface s=getTheme()->getSurface("window.border.image");
  int bw=s.getRect().w/3;
  int titBarHeight=20;

  //  cdebug("window_border:"<<s.width()<<"/"<<s.height());

  if(pTitle.length())
    {
      // use 4 rows
      addFixedRow(bw); // upper border
      addFixedRow(titBarHeight); // titletext and button
      //      addFixedRow(bw); // lower border below titletext
      addRow(1.0); 
      addFixedRow(bw); // lower lower below window
      addFixedColumn(bw);
      addColumn(1.0);
      addFixedColumn(bw);
      
      AGTable::addChild(0,0,new AGImage(this,AGPoint(0,0),s,true,AGRect(0,0,bw,bw)));
      AGTable::addChild(1,0,new AGImage(this,AGPoint(0,0),s,true,AGRect(bw,0,bw,bw)));
      AGTable::addChild(2,0,new AGImage(this,AGPoint(0,0),s,true,AGRect(2*bw,0,bw,bw)));
      
      AGImage *i1,*i2;
      AGWidget *t;

      AGTable::addChild(0,1,i1=new AGImage(this,AGPoint(0,0),s,true,AGRect(0,bw,bw,bw)));
      // title
      //      AGTable::addChild(1,1,new AGImage(this,s,AGRect(bw,0,bw,bw)));
      //      AGTable::addChild(1,1,new AGText(this,AGRect(0,0,40,20),pTitle,AGFont("Arial.ttf",14)));
      AGTable::addChild(1,1,t=getTitleBar());//new AGButton(this,AGRect(0,0,10,20),pTitle));

      //      t->setSurface(getTheme()->getSurface("close_button"));

      AGTable::addChild(2,1,i2=new AGImage(this,AGPoint(0,0),s,true,AGRect(2*bw,bw,bw,bw)));
      i1->setHeight(t->height());
      i2->setHeight(t->height());

      AGTable::addChild(0,2,new AGImage(this,AGPoint(0,0),s,true,AGRect(0,bw,bw,bw)));
      AGTable::addChild(2,2,new AGImage(this,AGPoint(0,0),s,true,AGRect(2*bw,bw,bw,bw)));
      
      AGTable::addChild(0,3,new AGImage(this,AGPoint(0,0),s,true,AGRect(0,2*bw,bw,bw)));
      AGTable::addChild(1,3,new AGImage(this,AGPoint(0,0),s,true,AGRect(bw,2*bw,bw,bw)));
      AGTable::addChild(2,3,new AGImage(this,AGPoint(0,0),s,true,AGRect(2*bw,2*bw,bw,bw)));
      
      AGTable::addChild(1,2,mClient=new AGImage(this,AGPoint(0,0),s,true,AGRect(bw,bw,bw,bw)));
    }
  else
    {
      // use 3 rows
      addFixedRow(bw);
      addRow(1.0);
      addFixedRow(bw);
      addFixedColumn(bw);
      addColumn(1.0);
      addFixedColumn(bw);
      
      AGTable::addChild(0,0,new AGImage(this,AGPoint(0,0),s,true,AGRect(0,0,bw,bw)));
      AGTable::addChild(1,0,new AGImage(this,AGPoint(0,0),s,true,AGRect(bw,0,bw,bw)));
      AGTable::addChild(2,0,new AGImage(this,AGPoint(0,0),s,true,AGRect(2*bw,0,bw,bw)));
      
      AGTable::addChild(0,1,new AGImage(this,AGPoint(0,0),s,true,AGRect(0,bw,bw,bw)));
      AGTable::addChild(2,1,new AGImage(this,AGPoint(0,0),s,true,AGRect(2*bw,bw,bw,bw)));
      
      AGTable::addChild(0,2,new AGImage(this,AGPoint(0,0),s,true,AGRect(0,2*bw,bw,bw)));
      AGTable::addChild(1,2,new AGImage(this,AGPoint(0,0),s,true,AGRect(bw,2*bw,bw,bw)));
      AGTable::addChild(2,2,new AGImage(this,AGPoint(0,0),s,true,AGRect(2*bw,2*bw,bw,bw)));
      
      AGTable::addChild(1,1,mClient=new AGImage(this,AGPoint(0,0),s,true,AGRect(bw,bw,bw,bw)));
    }

  arrange();
}

void AGWindow::addChild(AGWidget *w)
{
  // FIXME: change this somehow?
  AGWidget::addChild(w);
  //  mClient->addChild(w);
}

AGWidget *AGWindow::getClient()
{
  return mClient;
}

bool AGWindow::eventMouseButtonDown(const AGEvent *m)
{
  CTRACE;
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(!hasFocus())
    if(e)
      if(getScreenRect().contains(e->getMousePosition()))
	if(getParent())
	  {
	    getParent()->gainFocus(this);
	    return true;
	  }

  bool ret=AGTable::eventMouseButtonDown(m);

  if(e)
    if(getScreenRect().contains(e->getMousePosition()))
      return true; // eat - if it was here - always!!!
  return ret;

}

bool AGWindow::eventDragBy(const AGEvent *event,const AGPoint &pDiff)
{
  //  TRACE;
  setTop(top()+pDiff.y);
  setLeft(left()+pDiff.x);
  return true;
}

AGWidget *AGWindow::getTitleBar()
{
  AGTable *t=new AGTable(this,AGRect(0,0,0,20));//60,20));
  AGButton *closeButton;
  AGWidget *title;

  t->addRow(1.0);
  //  t->addFixedColumn();
  t->addColumn(1.0);
  t->addFixedColumn(20);//close button
  
  //  t->addChild(0,0,title=new AGButton(t,AGRect(0,0,0,0),mTitle));
  t->addChild(0,0,title=new AGCaption(t,AGRect(0,0,0,0),mTitle,getTheme()->getFont("window.title.font"),AGBackground("menu.background.normal")));
  t->addChild(1,0,closeButton=new AGButton(t,AGRect(0,0,20,20),"aa"));
  closeButton->setSurface(getTheme()->getSurface("window.buttons.close"),false);


  //  AGListener 
  closeButton->sigClick.connect(slot(this,&AGWindow::tryClose));
  // t->arrange();
  return t;
}

bool AGWindow::tryClose(const char*pName,const AGEvent *m)
{
  TRACE;
  hide();
  return false;
}


AGRect AGWindow::getClientRect() const
{
  return const_cast<AGWindow*>(this)->getClient()->getClientRect()+mClient->getRect().getPosition();
}
