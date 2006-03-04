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
#include "ag_layoutfactory.h"
#include <typeinfo>


AGRadioGroup::AGRadioGroup(AGWidget *p,const AGRect2 &r):
  AGWidget(p,r),
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






AGRadio::AGRadio(AGWidget *pParent,AGRect2 pRect):
  AGCheckBox(pParent,pRect),mGroup(0)
{
  // search mGroup
  AGWidget *w=pParent;
  AGRadioGroup *g=0;
  while(w && g==0)
    {
      g=dynamic_cast<AGRadioGroup*>(w);
      w=w->getParent();
    }
  if(g)
    mGroup=g;

  if(mGroup)
    mGroup->add(this);
}

AGRadio::~AGRadio()
{
  if(mGroup)
    mGroup->erase(this);
}

void AGRadio::setChecked(bool pChecked)
{
  bool c=isChecked();
  if(c!=pChecked)
    {
      AGCheckBox::setChecked(pChecked);
      
      if(pChecked)
	{
	  if(mGroup)
	    mGroup->eventChange(getName());
	}
    }
}

void AGRadio::deselect()
{
  setChecked(false);
}

void AGRadio::setGroup(AGRadioGroup *pGroup)
{
  mGroup=pGroup;
}

bool AGRadio::eventMouseClick(AGEvent *m)
{
  if(!isChecked())
    setChecked(true);

  return AGButton::eventMouseClick(m);
}





// AGRadioGroup creator
class AGRadioGroupLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(RadioGroup,"radioGroup")

  virtual AGWidget *create(AGWidget *pParent,const AGRect2 &pRect,const xmlpp::Node &pNode)
  {
    AGRadioGroup *l=new AGRadioGroup(pParent,pRect);

    return l;
  }
};
IMPLEMENT_COMPONENT_FACTORY(RadioGroup);

// AGRadio creator
class AGRadioLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(Radio,"radio")

  virtual AGWidget *create(AGWidget *pParent,const AGRect2 &pRect,const xmlpp::Node &pNode)
  {
    AGRadio *b=new AGRadio(pParent,pRect);
    std::string caption=pNode.get("caption");
    if(caption.length())
      b->setCaption(caption);

    std::string captionImage=pNode.get("caption-image");
    if(captionImage.length())
      b->setSurface(AGSurface::load(captionImage),false);
    if(pNode.get("enabled")=="false")
      b->setEnabled(false);
    if(pNode.get("theme").length())
      b->setTheme(pNode.get("theme"));
    if(pNode.get("checked")=="true")
      b->setChecked(true);
    return b;
  }
};
IMPLEMENT_COMPONENT_FACTORY(Radio);
