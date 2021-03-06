/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_layoutfactory.cc
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

#include "ag_layoutfactory.h"
#include "ag_debug.h"
#include "ag_kill.h"

AGLayoutCreator::AGLayoutCreator():mWidget(0)
{
}

void AGLayoutCreator::create(AGWidget *pParent,const AGRect2 &pRect,const Node &pNode)
{
}

void AGLayoutCreator::setResult(AGWidget *pWidget)
{
  mWidget=pWidget;
}
AGWidget *AGLayoutCreator::getResult()
{
  return mWidget;
}

void AGLayoutCreator::clearResult()
{
  mWidget=0;
}

void AGLayoutCreator::mark()
{
  if(mWidget)
    markObject(mWidget);
}



AGLayoutFactory::AGLayoutFactory()
{
  REGISTER_SINGLETON(this);
}

AGLayoutFactory::~AGLayoutFactory()
{
}

void AGLayoutFactory::addCreator(const AGString &pName,AGLayoutCreator *creator)
{
  mCreators[pName]=creator;
}

void AGLayoutFactory::removeCreator(const AGString &pName,AGLayoutCreator *creator)
{
  std::map<AGString,AGLayoutCreator*>::iterator i=mCreators.find(pName);
  if(i->second==creator)
    mCreators.erase(i);
}


AGWidget *AGLayoutFactory::create(AGWidget *pParent,const AGRect2 &pRect,const Node &pNode)
{
  AGLayoutCreator *creator=mCreators[pNode.getName()];

  if(creator)
    {
      AGWidget *w;
      creator->create(pParent,pRect,pNode);
      w=creator->getResult();
      creator->clearResult();

      return w;
    }
  std::string name;
  if(name!="" && name!="colsize" && name!="rowsize")
    {
      cdebug("no creation at:"<<name);
    }
  return 0;
}
  
AGLayoutFactory *mLayoutFactory=0;
AGLayoutFactory *getLayoutFactory()
{
  if(!mLayoutFactory)
    mLayoutFactory=new AGLayoutFactory;
  return mLayoutFactory;
}



