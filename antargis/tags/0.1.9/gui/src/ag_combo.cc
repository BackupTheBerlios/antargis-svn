/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_combo.cc
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

#include "ag_application.h"
#include "ag_button.h"
#include "ag_combo.h"
#include "ag_debug.h"
#include "ag_edit.h"
#include "ag_listbox.h"
#include "ag_theme.h"

AGComboBox::AGComboBox(AGWidget *pParent,const AGRect2 &pRect):
  AGWidget(pParent,pRect),sigSelect(this,"sigSelect")
{
  mEdit=new AGEdit(this,AGRect2(0,0,width()-height(),height()));
  mButton=new AGButton(this,AGRect2(width()-height(),0,height(),height()),"");

  mButton->setSurface(AGSurface::load("data/gui/arrow_down.png"));

  AGFont f=getTheme()->getFont("combobox.font");

  mEdit->setMutable(false);
  mEdit->setFont(f);
  addChild(mEdit);
  addChild(mButton);

  mButton->sigClick.connect(slot(this,&AGComboBox::eventButtonClicked));
}


void AGComboBox::insertItem(const std::string &pID,const std::string &pContent)
{
  ops.push_back(std::make_pair(pID,pContent));
  if(ops.size()==1)
    mID=pID;
  update();
}

void AGComboBox::update()
{
  if(ops.size()>0)
    {
      // find
      std::string text;
      for(std::list<std::pair<std::string,std::string> >::iterator i=ops.begin();i!=ops.end();i++)
	if(i->first==mID)
	  {
	    text=i->second;
	    break;
	  }

      mEdit->setText(text);
    }
}

bool AGComboBox::eventButtonClicked(AGEvent *pEvent)
{
  if(ops.size()==0)
    return true;

  AGRect2 sr=getScreenRect();

  cdebug(sr.x());
  cdebug(sr.y());
  cdebug(width());
  cdebug(getTheme()->getInt("listbox.item.height")*std::min(8,(int)ops.size()));

  float mItemHeight=getTheme()->getInt("listbox.item.height");
  if(mItemHeight<5)
    mItemHeight=25;


  mListBox=new AGListBox(0,AGRect2(sr.x(),sr.y()+height(),width(),mItemHeight*8));

  mListBox->sigSelect.connect(slot(this,&AGComboBox::eventSelected));

  for(std::list<std::pair<std::string,std::string> >::iterator i=ops.begin();i!=ops.end();i++)
    mListBox->insertItem(i->first,i->second);
  if(ops.size()>0)
    mListBox->selectItem(mID);

  getApplication()->setOverlay(mListBox);
  return true;
}

bool AGComboBox::eventSelected(AGEvent *pEvent)
{
  CTRACE;
  mID=mListBox->getSelectedID();
  update();
  getApplication()->setOverlay(0);

  sigSelect(new AGEvent(this,"sigSelect"));
  return true;
}

std::string AGComboBox::getSelected() const
{
  return mID;
}
void AGComboBox::setSelected(const std::string &pID)
{
  mID=pID;
  update();
}

void AGComboBox::clear()
{
  ops.clear();
  mID="";
  update();
}
