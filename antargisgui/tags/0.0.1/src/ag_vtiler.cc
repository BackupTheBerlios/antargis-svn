/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_vtiler.cc
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

#include "ag_vtiler.h"
#include "ag_debug.h"

AGVTiler::AGVTiler(AGWidget *pParent,const AGRect&pRect,bool pAdaptMyHeight):
  AGWidget(pParent,pRect),mAdaptMyHeight(pAdaptMyHeight)

{
}

void AGVTiler::addChild(AGWidget *pWidget)
{
  AGWidget::addChildBack(pWidget);
  rePosition();
}
void AGVTiler::rePosition()
{
  // recalc sizes
  
  // always adapt to max width
  int w=0,h=0,y=0;//getRect().y;
  std::list<AGWidget*>::const_iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    {
      w=std::max(w,(*i)->width());
      h+=(*i)->height();
    }

  std::list<AGWidget*>::iterator j=mChildren.begin();
  for(;j!=mChildren.end();j++)
    {
      (*j)->setWidth(w);
      (*j)->setTop(y);
      (*j)->setLeft(0);//left());
      y+=(*j)->height();
      //cdebug(y);
    }
  if(mAdaptMyHeight)
    setHeight(h);
}


void AGVTiler::adaptWidthFromChildren()
{
  int w=0;
  std::list<AGWidget*>::const_iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    {
      w=std::max(w,(*i)->width());
      //      w+=(*i)->width();
    }
  setWidth(w);
}

void AGVTiler::adaptHeightFromChildren()
{
  int h=0;
  std::list<AGWidget*>::const_iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    {
      //h=std::max(h,(*i)->height());
      h+=(*i)->height();
    }
  setHeight(h);
}



/////////////////////////////////////////////////////////
// AVHTiler
/////////////////////////////////////////////////////////



AGHTiler::AGHTiler(AGWidget *pParent,const AGRect&pRect,bool pAdaptMyHeight):
  AGVTiler(pParent,pRect,pAdaptMyHeight)
{
}


void AGHTiler::rePosition()
{
  // recalc sizes
  
  // always adapt to max width
  int w=0,h=0,x=0;//getRect().x;
  std::list<AGWidget*>::const_iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    {
      h=std::max(h,(*i)->height());
      w+=(*i)->width();
    }

  std::list<AGWidget*>::iterator j=mChildren.begin();
  for(;j!=mChildren.end();j++)
    {
      (*j)->setHeight(h);
      (*j)->setLeft(x);
      (*j)->setTop(0);//top());
      x+=(*j)->width();
    }
  
  if(mAdaptMyHeight)
    setWidth(w);
}


void AGHTiler::adaptHeightFromChildren()
{
  int h=0;
  std::list<AGWidget*>::const_iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    {
      h=std::max(h,(*i)->height());
      //      w+=(*i)->width();
    }
  setHeight(h);
}

void AGHTiler::adaptWidthFromChildren()
{
  int w=0;
  std::list<AGWidget*>::const_iterator i=mChildren.begin();
  for(;i!=mChildren.end();i++)
    {
      //h=std::max(h,(*i)->height());
      w+=(*i)->width();
    }
  setWidth(w);
}

void AGHTiler::adaptWidthFromParent()
{
  AGWidget *w=getParent();
  if(w)
    setWidth(w->width());
}
