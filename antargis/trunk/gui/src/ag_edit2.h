/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_edit2.h
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

#ifndef __AG_EDIT2_H
#define __AG_EDIT2_H

#include <ag_widget.h>

class AGEdit2Content;

class AGEdit2:public AGWidget
{
 public:
  AGEdit2(AGWidget *pParent,const AGRect2 &pRect);
  ~AGEdit2();

  void setText(const std::string &pText);
  void setCursor(size_t i);


  void setSelection(size_t start,size_t len);

 private:

  void checkCursor();

  AGEdit2Content *mContent;

  size_t mCursor;
};


#endif
