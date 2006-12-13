/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_layoutfactory.h
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

#ifndef AG_LAYOUTFACTORY_H
#define AG_LAYOUTFACTORY_H

#include <ag_singleton.h>
#include <ag_utf8.h>
#include <ag_widget.h>
#include <ag_xml.h>

class AGLayoutCreator:public AGSingleton
{
 public:
  AGLayoutCreator();//const AGString &pName);
  //  virtual ~AGLayoutCreator();
  virtual void create(AGWidget *pParent,const AGRect2 &pRect,const Node &pNode);
  void setResult(AGWidget *pWidget);
  AGWidget *getResult();

  void clearResult();

  void mark();
 private:
  AGWidget *mWidget;
    //std::string mName;

};

class AGLayoutFactory
{
  std::map<AGString,AGLayoutCreator*> mCreators;

  AGLayoutFactory();
 public:
  ~AGLayoutFactory();

  void addCreator(const AGString &pName,AGLayoutCreator *creator);
  void removeCreator(const AGString &pName,AGLayoutCreator *creator);

  AGWidget *create(AGWidget *pParent,const AGRect2 &pRect,const Node &pNode);
  
  friend AGLayoutFactory *getLayoutFactory();
};

AGLayoutFactory *getLayoutFactory();
/*
#define IMPLEMENT_COMPONENT_FACTORY(CLASS) \
AG##CLASS##LayoutCreator factory_##CLASS

#define REGISTER_COMPONENT(CLASS,NAME) AG##CLASS##LayoutCreator():AGLayoutCreator(NAME){}
*/

#define IMPLEMENT_COMPONENT_FACTORY(CLASS)
#define REGISTER_COMPONENT(CLASS,NAME)


#endif
