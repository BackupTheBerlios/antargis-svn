/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_dialog.cc
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

#include "ag_dialog.h"

AGDialog::AGDialog(AGWidget *pgParent,const std::string &pXMLData):
  AGLayout(pgParent,pXMLData)
{
  AGWidget *w;
  
  w=getChild("ok");
  if(w)
    w->sigClick.connect(slot(this,&AGDialog::eventOk));
  w=getChild("cancel");
  if(w)
    w->sigClick.connect(slot(this,&AGDialog::eventCancel));
  w=getChild("close");
  if(w)
    w->sigClick.connect(slot(this,&AGDialog::eventClose));
  
}

bool AGDialog::eventOk(const char*pName,const AGEvent *event,AGMessageObject *pCaller)
{
  CTRACE;
  return false;
}
bool AGDialog::eventCancel(const char*pName,const AGEvent *event,AGMessageObject *pCaller)
{
  CTRACE;
  return false;
}
bool AGDialog::eventClose(const char*pName,const AGEvent *event,AGMessageObject *pCaller)
{
  CTRACE;
  return false;
}
