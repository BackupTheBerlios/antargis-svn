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

AGCheckBox::AGCheckBox(AGWidget *pParent,AGRect pRect):
  AGButton(pParent,pRect,"")//,mChecked(false)
{
  /*  mImage=0;
  if(getTheme()->hasSurface(mType+".normal"))
    addChild(mImage=new AGImage(this,AGPoint(0,0),getTheme()->getSurface(mType+".normal"),false));
  addChild(new AGText(this,AGPoint(0,0),pName,getTheme()->getFont(std::string("Font.")+mType)));

  adaptHeightFromChildren();*/
}

bool AGCheckBox::eventMouseClick(const AGEvent *m)
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
