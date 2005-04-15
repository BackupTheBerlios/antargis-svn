/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_dialog.h
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

#ifndef AG_DIALOG_H
#define AG_DIALOG_H

#include "ag_layout.h"

class AGDialog:public AGLayout
{
 public:
  AGDialog(AGWidget *pgParent,const std::string &pXMLData);

  virtual bool eventOk(const char*pName,const AGEvent *event,AGMessageObject *pCaller);
  virtual bool eventCancel(const char*pName,const AGEvent *event,AGMessageObject *pCaller);
  virtual bool eventClose(const char*pName,const AGEvent *event,AGMessageObject *pCaller);
};

#endif
