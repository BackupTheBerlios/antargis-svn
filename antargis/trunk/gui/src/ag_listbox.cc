/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_listbox.cc
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

#include "ag_listbox.h"
#include "ag_edit.h"
#include "ag_theme.h"
#include "ag_debug.h"

#include <sstream>

AGListBoxItem::AGListBoxItem(std::string pID,std::string pValue)
{
  id=pID;
  value=pValue;
}



AGListBox::AGListBox(AGWidget *pParent,const AGRect2 &pRect):AGWidget(pParent,pRect)
{
  mBackground=AGBackground("listbox.background");
  mHilight=AGBackground("listbox.selected");
  // insert AGEdits
  int y=0;
  int count=0;
  mItemHeight=getTheme()->getInt("listbox.item.height");
  if(mItemHeight<5)
    mItemHeight=25;

  AGFont f=getTheme()->getFont("listbox.font");

  for(;y<pRect.h();y+=mItemHeight,count++)
    {
      AGRect2 r(0,y,pRect.w(),mItemHeight);
      cdebug(r);
      AGEdit *e=new AGEdit(this,r);
      e->setMutable(false);
      e->setBackground(false);
      e->setFont(f);
      std::ostringstream os;
      os<<"ListBoxItem"<<count;
      e->setName(os.str());

      mEdits.push_back(e);
      addChild(e);
    }
  mHeight=count;
  mY=0;
  mSelected=-1;
}

void AGListBox::insertItem(std::string pID,std::string pValue)
{
  mItems.push_back(AGListBoxItem(pID,pValue));
  arrange();
}
void AGListBox::selectItem(std::string pID)
{
  for(size_t i=0;i<mItems.size();i++)
    if(mItems[i].id==pID)
      {
	mSelected=i;
	mY=std::max(0,int(i)-mHeight/2); // FIXME: maybe find better zoom
      }
  arrange();
}

std::string AGListBox::getSelectedID() const
{
  if(mSelected>=0 && size_t(mSelected)<mItems.size())
    return mItems[mSelected].id;
  return "";
}

std::string AGListBox::getSelectedValue() const
{
  if(mSelected>=0 && size_t(mSelected)<mItems.size())
    return mItems[mSelected].id;
  return "";
}

bool AGListBox::eventKeyDown(AGEvent *m)
{
  if(m->isSDLEvent())
    {
      SDLKey k=m->getKey();
      if(k==SDLK_UP)
	{
	  if(mSelected<2 && mItems.size()>0)
	    mSelected=0;
	  else
	    mSelected--;
	  if(mSelected<mY)
	    mY=mSelected;
	  arrange();
	  return true;
	}
      else if(k==SDLK_DOWN)
	{
	  if(mSelected<int(mItems.size())-1)
	    mSelected++;
	  if(mSelected>=mY+mHeight)
	    mY++;
	  arrange();
	  return true;
	}
    }
  return AGWidget::eventKeyDown(m);
}


void AGListBox::arrange()
{
  size_t y,y2=0;
  
  // first clear all
  for(y=0;y<mEdits.size();y++)
    mEdits[y]->setText("");

  for(y=mY,y2=0;y<(size_t)(mY+mHeight) && y<mItems.size();y++,y2++)
    mEdits[y2]->setText(mItems[y].value);
}

void AGListBox::draw(AGPainter &p)
{
  mBackground.draw(getRect().origin(),p);
  p.pushMatrix();

  if(mSelected>=0 && mSelected<(int)mItems.size())
    {
      int y=mSelected-mY;
      
      AGRect2 r(0,y*mItemHeight,width(),mItemHeight);
      p.transform(r);
      mHilight.draw(getRect().origin(),p);
    }
  p.popMatrix();
  AGWidget::draw(p);
}

bool AGListBox::eventMouseClick(AGEvent *e)
{
  CTRACE;
  if(e->isSDLEvent())
    {
      AGVector2 p=e->getMousePosition();

      int b=e->getButton();
      cdebug(b);
      if(b==4)
	{
	  // up wheel
	  if(mY>0)
	    mY--;
	  arrange();
	  return true;
	}
      else if(b==5)
	{
	  // down wheel
	  if(mY<=mItems.size()-mHeight)
	    mY++;
	  arrange();
	  return true;
	}
      else
	{

	  int y=p[1]-getScreenRect().y();
	  cdebug("y:"<<y);
	  y/=mItemHeight;
	  cdebug("y:"<<y);
	  cdebug(mItemHeight);
	  int n=y+mY;
	  cdebug("n:"<<n);
	  if(n<mItems.size())
	    mSelected=n;
	  sigSelect(e);
	  return true;
	}
    }
  return false;
}

AGListBox &toAGListBox(AGWidget &w)
{
  return dynamic_cast<AGListBox&>(w);
}

void AGListBox::clearList()
{
  mY=mSelected=0;
  mItems.clear();
}
