/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_radio.cc
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


#include "ag_radio.h"
#include "ag_theme.h"
#include "ag_image.h"




AGRadioGroup::AGRadioGroup():
  sigChanged(this,"sigChanged")
{
}
AGRadioGroup::~AGRadioGroup()
{
}

void AGRadioGroup::eventChange(const std::string &p)
{
  std::set<AGRadio*>::iterator i=mChildren.begin();

  for(;i!=mChildren.end();i++)
    {
      if((*i)->getName()!=p)
	(*i)->deselect();
    }

}

void AGRadioGroup::add(AGRadio *r)
{
  mChildren.insert(r);
}
void AGRadioGroup::erase(AGRadio *r)
{
  mChildren.erase(r);
}









AGRadio::AGRadio(AGWidget *pParent,AGRect pRect,AGRadioGroup *pGroup,std::string pName):
  AGCheckBox(pParent,pRect,pName,"radioButton"),mGroup(pGroup)
{
  if(mGroup)
    mGroup->add(this);
}

AGRadio::~AGRadio()
{
  if(mGroup)
    mGroup->erase(this);
}


bool AGRadio::eventMouseClick(const AGEvent *m)
{
  if(!mChecked)
    {
      mChecked=true;
      mImage->setSurface(getTheme()->getSurface(mType+".checked"));
      if(mGroup)
	mGroup->eventChange(getName());
    }
  return true;
}

void AGRadio::deselect()
{
  mImage->setSurface(getTheme()->getSurface(mType+".normal"));
  mChecked=false;
}
