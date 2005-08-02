/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_text.h
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

#ifndef AG_TEXT_H
#define AG_TEXT_H

#include "ag_widget.h"
#include "ag_font.h"

class AGText:public AGWidget
{
 public:
  AGText(AGWidget *pParent,const AGRect &pRect,const std::string &pText,const AGFont &pFont);
#ifndef SWIG
  AGText(AGWidget *pParent,const AGPoint &pPoint,const std::string &pText,const AGFont &pFont);
#endif

  void setDeriveRect();
  virtual void draw(AGPainter &p);

  void setText(const std::string &pText);
  std::string getText() const;

 private:
  std::string mText;
  AGFont mFont;
  bool mFixedSize;
};

AGText &toAGText(AGWidget &w);

#endif
