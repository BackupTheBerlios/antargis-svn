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

AGLayoutCreator::AGLayoutCreator(const std::string &pName)
{
  getLayoutFactory()->addCreator(pName,this);
}


AGLayoutFactory::AGLayoutFactory()
{
}

void AGLayoutFactory::addCreator(const std::string &pName,AGLayoutCreator *creator)
{
  mCreators[pName]=creator;
}

AGWidget *AGLayoutFactory::create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
{
  cdebug(pNode.getName());
  if(mCreators[pNode.getName()])
    return mCreators[pNode.getName()]->create(pParent,pRect,pNode);
  cdebug("not found");
  return 0;
}
  
AGLayoutFactory *mLayoutFactory=0;
AGLayoutFactory *getLayoutFactory()
{
  if(!mLayoutFactory)
    mLayoutFactory=new AGLayoutFactory;
  return mLayoutFactory;
}



