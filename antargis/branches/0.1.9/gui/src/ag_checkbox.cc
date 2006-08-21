/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_checkbox.cc
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

#include "ag_checkbox.h"
#include "ag_image.h"
#include "ag_text.h"
#include "ag_theme.h"
#include "ag_debug.h"
#include "ag_layoutfactory.h"
#include "ag_local.h"

AGCheckBox::AGCheckBox(AGWidget *pParent,AGRect2 pRect):
  AGButton(pParent,pRect,"")//,mChecked(false)
{
  mSurfaces[0]=mSurfaces[1]=0;
  /*  mImage=0;
  if(getTheme()->hasSurface(mType+".normal"))
    addChild(mImage=new AGImage(this,AGVector2(0,0),getTheme()->getSurface(mType+".normal"),false));
  addChild(new AGText(this,AGVector2(0,0),pName,getTheme()->getFont(std::string("Font.")+mType)));

  adaptHeightFromChildren();*/
}

bool AGCheckBox::eventMouseClick(AGEvent *m)
{
  CTRACE;
  setChecked(!isChecked());
  /*
  mChecked=!mChecked;
  if(mChecked)
    setState(CHECKED);
  else
  setState(NORMAL);*/
    /*

  if(mImage)
    {
      if(mChecked)
	mImage->setSurface(getTheme()->getSurface(mType+".checked"));
      else
	mImage->setSurface(getTheme()->getSurface(mType+".normal"));
	}*/
  return AGButton::eventMouseClick(m);//false;//true; // eat
}

/*
std::string AGCheckBox::getName() const
{
  return mName;
}
*/


void AGCheckBox::setSurfaces(AGSurface pDisabledSurface,AGSurface pEnabledSurface)
{
  delete mSurfaces[0];
  delete mSurfaces[1];
  mSurfaces[0]=new AGSurface(pDisabledSurface);
  mSurfaces[1]=new AGSurface(pEnabledSurface);

  //  setState(getState());
  queryRedraw();
}

void AGCheckBox::setState(const State &pState)
{
  if(mSurfaces[0])
    {
      if(pState==NORMAL || pState==LIGHTED || pState==PRESSED)
	setSurface(*(mSurfaces[0]));
      else
	setSurface(*(mSurfaces[1]));
    }

  AGButton::setState(pState);
}



// AGRadio creator
class AGCheckBoxLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(CheckBox,"checkBox")

  virtual AGWidget *create(AGWidget *pParent,const AGRect2 &pRect,const Node &pNode)
  {
    AGCheckBox *b=new AGCheckBox(pParent,pRect);
    std::string caption=_(pNode.get("caption"));
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

    if(pNode.get("disabledImage")!="" && pNode.get("enabledImage")!="")
      {
	b->setSurfaces(AGSurface::load(pNode.get("disabledImage")),AGSurface::load(pNode.get("enabledImage")));
      }
      


    return b;
  }
};
IMPLEMENT_COMPONENT_FACTORY(CheckBox);
