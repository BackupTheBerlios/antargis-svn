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
class AGImage;

/**
   AGButton represents a button, which can hold a text-caption or an image-caption, but not both.
   Further it can have following states:
*/

class AGButton:public AGWidget
{
 public:
  /// the different drawing states
  enum State {NORMAL,LIGHTED,PRESSED,CHECKED,CHECKEDLIGHTED,CHECKEDPRESSED,DISABLED};

  AGButton(AGWidget *pParent,const AGRect2 &r,const std::string&pText,int id=-1);

  void setSurface(AGSurface pSurface,bool pChangeSize=false);
  void setTexture(const AGTexture &pTexture);

  virtual void draw(AGPainter &p);

  virtual bool eventMouseEnter();
  virtual bool eventMouseLeave();

  virtual bool eventMouseButtonDown(AGEvent *m);
  virtual bool eventMouseButtonUp(AGEvent *m);

  virtual void setWidth(float w);
  virtual void setHeight(float w);

  virtual std::string getCaption() const;

  void setEnabled(bool pEnable);

  void setTheme(const std::string &pTheme);

  void setCaption(const std::string &pCaption);

  /// sets the current state and ensures a redraw
  virtual void setState(const State &pState);
  State getState() const;

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
  AGSurface mGrayedSurface;
  bool mHasSurface;
  AGEdit *mTextW;
  AGImage *mImageW;

  bool lower;
  bool mChecked;

  std::map<State,AGBackground> mBG;
  std::map<State,AGBorder> mBorder;
  bool mEnabled;
};

AGButton &toAGButton(AGWidget &pWidget);

#endif
