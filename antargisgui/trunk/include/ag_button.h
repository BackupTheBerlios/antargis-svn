#ifndef AG_BUTTON_H
#define AG_BUTTON_H

#include "ag_widget.h"
#include "ag_surface.h"
#include "ag_background.h"

#include <iostream>
#include <map>

class AGText;

class AGButton:public AGWidget
{
 public:
  enum State {NORMAL,LIGHTED,PRESSED,CHECKED};

  AGButton(AGWidget *pParent,const AGRect &r,const std::string&pText,int id=-1,const std::string&pStyle="button");
  //  AGButton(AGWidget *pParent,const AGRect &r,const AGSurface &pSurface);
  //  AGButton();

  void setSurface(AGSurface pSurface,bool pChangeSize=true);

  virtual void draw(const AGRect &r);

  //  virtual void drawBackground(const AGRect &r);

  virtual bool eventMouseEnter();
  virtual bool eventMouseLeave();

  virtual bool eventMouseButtonDown(const AGEvent *m);
  virtual bool eventMouseButtonUp(const AGEvent *m);

  virtual void setWidth(int w);
  virtual void setHeight(int w);

 private:
  std::string mText;
  int mID;
  std::string mStyle;
  State mState;
  int borderWidth;
  AGSurface mSurface;
  bool mHasSurface;
  AGText *mTextW;

  bool checkButton;
  bool checked;

  bool lower;

  std::map<State,AGBackground> mBG;
};

#endif
