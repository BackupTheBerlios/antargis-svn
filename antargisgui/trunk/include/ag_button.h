/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_button.h
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

#ifndef AG_BUTTON_H
#define AG_BUTTON_H

#include "ag_widget.h"
#include "ag_surface.h"
#include "ag_background.h"

#include <iostream>
#include <map>

class AGEdit;

/**
  TODO: redesign, so that button takes bg-images or border/background-style and a caption of type AGWidget 
*/

class AGButton:public AGWidget
{
 public:
  enum State {NORMAL,LIGHTED,PRESSED,CHECKED};

  AGButton(AGWidget *pParent,const AGRect &r,const std::string&pText,int id=-1);

  void setSurface(AGSurface pSurface,bool pChangeSize=true);

  virtual void draw(AGPainter &p);

  //  virtual void drawBackground(const AGRect &r);

  virtual bool eventMouseEnter();
  virtual bool eventMouseLeave();

  virtual bool eventMouseButtonDown(const AGEvent *m);
  virtual bool eventMouseButtonUp(const AGEvent *m);

  virtual void setWidth(int w);
  virtual void setHeight(int w);

  virtual std::string getName() const;

  void setEnabled(bool pEnable);

  void setTheme(const std::string &pTheme);

 private:
  std::string mText;
  int mID;
  std::string mTheme;
  State mState;
  int borderWidth;
  AGSurface mSurface;
  bool mHasSurface;
  AGEdit *mTextW;

  bool checkButton;
  bool checked;

  bool lower;

  std::map<State,AGBackground> mBG;
  bool mEnabled;
};

#endif
