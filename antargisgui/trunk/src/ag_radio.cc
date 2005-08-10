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


AGRadioGroup::AGRadioGroup(AGWidget *p,const AGRect &r):
  AGWidget(p,r),
  sigChanged(this,"sigChanged")
{
}
AGRadioGroup::~AGRadioGroup()
{
}

void AGRadioGroup::eventChange(const std::string &p)
{
  CTRACE;
  std::set<AGRadio*>::iterator i=mChildren.begin();

  for(;i!=mChildren.end();i++)
    {
      cdebug((*i)->getName());
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









AGRadio::AGRadio(AGWidget *pParent,AGRect pRect)://,AGRadioGroup *pGroup,std::string pName):
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
  cdebug("G:"<<g);
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

/*
bool AGRadio::eventMouseButtonUp(const AGEvent *m)
{
  CTRACE;
  cdebug("name:"<<getName());
  if(!mChecked)
    {
      mChecked=true;
      //      mImage->setSurface(getTheme()->getSurface(mType+".checked"));
      if(mGroup)
	mGroup->eventChange(getName());
    }

  cdebug(mChecked);
  if(mChecked)
    setState(CHECKED);
  else
    setState(NORMAL);
  return true;
}

bool AGRadio::eventMouseLeave()
{
  if(mChecked)
    setState(CHECKED);
  else
    setState(NORMAL);
  return false;
}
bool AGRadio::eventMouseEnter()
{
  if(mChecked)
    setState(CHECKED);
  else
    setState(LIGHTED);
  return false;
}
*/

bool AGRadio::eventMouseClick(const AGEvent *m)
{
  CTRACE;
  if(!isChecked())
    {
      setChecked(true);
      cdebug("name:"<<getName());
      cdebug("type:"<<typeid(*this).name());
      //      mChecked=true;
      //      mImage->setSurface(getTheme()->getSurface(mType+".checked"));
      if(mGroup)
	mGroup->eventChange(getName());
    }

  cdebug(isChecked());
  /*  if(mChecked)
    setState(CHECKED);
  else
  setState(LIGHTED);*/
  return AGWidget::eventMouseClick(m);
}

void AGRadio::deselect()
{
  CTRACE;
  //  mImage->setSurface(getTheme()->getSurface(mType+".normal"));
  setChecked(false);
  //  mChecked=false;
  //  setState(NORMAL);
}


// AGRadioGroup creator
class AGRadioGroupLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(RadioGroup,"radioGroup")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    CTRACE;
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

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    CTRACE;
    AGRadio *b=new AGRadio(pParent,pRect);
    std::string captionImage=pNode.get("caption-image");
    if(captionImage.length())
      b->setSurface(getSurfaceManager()->loadSurface(captionImage),false);
    if(pNode.get("enabled")=="false")
      b->setEnabled(false);
    if(pNode.get("theme").length())
      b->setTheme(pNode.get("theme"));

    return b;
  }
};
IMPLEMENT_COMPONENT_FACTORY(Radio);
