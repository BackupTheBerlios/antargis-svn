/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_edit.h
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

#ifndef __AG_EDIT_H
#define __AG_EDIT_H

#include "ag_widget.h"
#include "ag_font.h"
#include "ag_surface.h"
#include "ag_background.h"

enum AGAlign {EDIT_LEFT,EDIT_RIGHT,EDIT_JUSTIFY,EDIT_CENTER};
enum AGVAlign {EDIT_TOP,EDIT_BOTTOM,EDIT_VCENTER};

class AGEditLine
{
 public:
  AGEditLine(const std::string &pText,AGFont pFont,bool pHardEnd);
  virtual ~AGEditLine();

  void setAlign(AGAlign pAlign);
  void setVAlign(AGVAlign pVAlign);

  virtual void draw(AGPainter &p,const AGPoint &pPoint,const AGRect &pClip);
  virtual void drawCursor(AGPainter &p,int cx,const AGPoint &pPoint,const AGRect &pClip,const AGColor &c);
  
  int height() const;
  int width() const;

  void insert(char c,int cx);
  void doDelete(int cx);
  AGEditLine split(int cx);

  int length() const;

  void append(std::string s);
  void prepend(std::string s);
  std::string getText() const;
  AGFont getFont() const;
  void setText(const std::string &s);

  bool hardEnd() const;
  void setHardEnd(bool pHardEnd);
  std::pair<std::string,bool> AGEditLine::checkUnwrap(int pW,std::string s);

  std::pair<std::string,bool> AGEditLine::checkWrap(int pW);

  void setFont(const AGFont &pFont);

 private:
  std::string mText;
  AGFont mFont;
  bool mHardEnd;
  AGAlign mAlign;
  AGVAlign mVAlign;
};

class AGEdit:public AGWidget
{
 public:
  AGEdit(AGWidget *pParent,const AGRect &pRect);

  virtual void draw(AGPainter &p);
  virtual void drawBackground(AGPainter &p);

  virtual bool eventKeyDown(AGEvent *m);
  virtual bool eventKeyUp(AGEvent *m);

  void setMulti(bool mMultiLine);

  bool insert(char c);
  void doDelete(int p);

  void setText(const std::string &pText);
  void setFont(const AGFont &pFont);

  void clear();

  void setMutable(bool pMutable);

  void setAlign(AGAlign pAlign);
  void setVAlign(AGVAlign pVAlign);

  void setBackground(bool pDrawBackground);
  void setBackground(const AGBackground &bg);

  void setTheme(const std::string &s);

  bool canFocus() const;

  std::string getText() const;

  void setMaxLength(int i);
 private:

  void getActLine();
  void insertLine(AGEditLine l);
  void mergeLine(int p);
  void AGEdit::checkWrap();

  std::list<AGEditLine> mLines;
  AGEditLine *actLine;
  
  int mCx,mCy; // cursor position
  int mViewCy;
  
  Uint32 mCursorTime;
  Uint32 mCursorLast;
  bool mShowCursor;
  bool mLShift,mRShift;
  bool mLCtrl,mRCtrl;
  bool mLAlt,mRAlt;

  bool mMultiLine;

  bool mWrapLines;

  AGBackground mBackground;

  AGFont mFont;

  AGAlign mAlign;
  AGVAlign mVAlign;

  bool mMutable;
  bool mDrawBackground;

  std::string mTheme;

  int mMaxLength;
};

AGEdit &toAGEdit(AGWidget &w);

#endif
