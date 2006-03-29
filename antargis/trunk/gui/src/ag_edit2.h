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
#include <ag_surface.h>
#include <ag_background.h>

class AGEdit2Content;

enum AGAlign {EDIT_LEFT,EDIT_RIGHT,EDIT_JUSTIFY,EDIT_CENTER};
enum AGVAlign {EDIT_TOP,EDIT_BOTTOM,EDIT_VCENTER};

class AGEdit2:public AGWidget
{
 public:
  typedef char Char;


  AGEdit2(AGWidget *pParent,const AGRect2 &pRect);
  ~AGEdit2();

  void clear();
  void setText(const std::string &pText);
  std::string getText() const;

  void setMutable(bool pMutable);
  void setMaxLength(int i);

  void setCursor(size_t i);

  void setAlign(AGAlign pAlign);
  void setVAlign(AGVAlign pVAlign);

  void setMulti(bool mMultiLine);


  void setBackground(bool pDrawBackground);
  void setBackground(const AGBackground &bg);

  bool canFocus() const;

  void setSelection(size_t start,size_t len);
  void assignFontToSelection(AGFont &pFont);
  void clearSelection();

  void assignFont(AGFont &pFont);

  // drawing
  virtual void prepareDraw();

  virtual void draw(AGPainter &p);
  virtual void drawBackground(AGPainter &p);


  // events
  virtual bool eventGotFocus();
  virtual bool eventLostFocus();

  virtual bool eventKeyDown(AGEvent *m);
  virtual bool eventKeyUp(AGEvent *m);


 private:
  void cancelSound();
  void insertChar(Char c);
  void insertWhiteSpace();

  void checkCursor();
  void checkSelection();

  AGEdit2Content *mContent;

  size_t mCursor;
  size_t mSelBegin,mSelSize;
  bool mInserting;
  size_t mMaxLen;

  Uint32 mCursorTime;
  Uint32 mCursorLast;
  bool mShowCursor;

  bool mMutable;
};


#endif
