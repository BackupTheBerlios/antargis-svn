/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_colorbutton.cc
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

#include "ag_colorbutton.h"
#include "ag_layoutfactory.h"

AGColorButton::AGColorButton(AGWidget *pParent,const AGRect &r,int x,int y):
  AGWidget(pParent,r),gridx(x),gridy(y),mColors(x*y)
{
}

void AGColorButton::draw(AGPainter &p)
{
  AGWidget::draw(p);
  
  int i=0;
  AGRect ar(getRect().origin());
  int w=ar.w/gridx;
  int h=ar.h/gridy;
  for(int y=0;y<gridy;y++)
    for(int x=0;x<gridx;x++)
      {
	AGRect r(ar.w*x/gridx,ar.h*y/gridy,w,h);
	p.drawRect(r,mColors[i++]);
      }
}

AGColor AGColorButton::getColor() const
{
  return mColor;
}

void AGColorButton::setColor(const AGColor &c)
{
  mColor=c;
}
void AGColorButton::setColor(int x,int y,const AGColor &c)
{
  assert(x>=0);
  assert(y>=0);
  assert(x<gridx);
  assert(y<gridy);
  mColors[x+y*gridx]=c;
}


AGColorButton &toAGColorButton(AGWidget &pWidget)
{
  return dynamic_cast<AGColorButton&>(pWidget);
}

bool AGColorButton::eventMouseClick(const AGEvent *m)
{
  const AGSDLEvent *e=dynamic_cast<const AGSDLEvent*>(m);
  if(e)
    {
      AGPoint p=e->getMousePosition()-getScreenRect().getPosition();
      int gx=p.x*gridx/getRect().w;
      int gy=p.y*gridy/getRect().h;
      int d=gx+gy*gridx;
      cdebug(p.x<<"   "<<p.y);
      cdebug("gx:"<<gx<<"  gy:"<<gy);
      assert(d<gridx*gridy && d>=0);
      mColor=mColors[d];
      cdebug(mColor.toString());
    }

  return AGWidget::eventMouseClick(m);
}


// Layout-Factories
class AGColorButtonLayoutCreator:public AGLayoutCreator
{
 public:
  REGISTER_COMPONENT(ColorButton,"colorButton")
  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    AGColorButton *b;

    b=new AGColorButton(pParent,pRect,toInt(pNode.get("gridx")),toInt(pNode.get("gridy")));
    
    //    if(pNode.get("enabled")=="false")
    //      b->setEnabled(false);
    //    if(pNode.get("theme").length())
    //      b->setTheme(pNode.get("theme"));
    return b;
  }
};
IMPLEMENT_COMPONENT_FACTORY(ColorButton);
