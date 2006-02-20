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
#include "ag_border.h"

#include <iostream>
#include <map>

class AGEdit;

/**
  TODO: redesign, so that button takes bg-images or border/background-style and a caption of type AGWidget 
*/

class AGButton:public AGWidget
{
 public:
  enum State {NORMAL,LIGHTED,PRESSED,CHECKED,CHECKEDLIGHTED,CHECKEDPRESSED,DISABLED};

  AGButton(AGWidget *pParent,const AGRect2 &r,const std::string&pText,int id=-1);

  void setSurface(AGSurface pSurface,bool pChangeSize=true);

  virtual void draw(AGPainter &p);

  //  virtual void drawBackground(const AGRect2 &r);

  virtual bool eventMouseEnter();
  virtual bool eventMouseLeave();

  virtual bool eventMouseButtonDown(AGEvent *m);
  virtual bool eventMouseButtonUp(AGEvent *m);

  virtual void setWidth(int w);
  virtual void setHeight(int w);

  virtual std::string getCaption() const;

  void setEnabled(bool pEnable);

  void setTheme(const std::string &pTheme);

  void setCaption(const std::string &pCaption);

  void setState(const State &pState);

  virtual void setChecked(bool pChecked);
  bool isChecked() const;

  bool canFocus() const;

  virtual void useTextures();

 private:
  std::string mText;
  int mID;
  std::string mTheme;
  State mState;
  int borderWidth;
  AGSurface mSurface;
  bool mHasSurface;
  AGEdit *mTextW;

  //  bool checkButton;
  //  bool checked;

  bool lower;
  bool mChecked;

  std::map<State,AGBackground> mBG;
  std::map<State,AGBorder> mBorder;
  bool mEnabled;
};

AGButton &toAGButton(AGWidget &pWidget);

#endif
