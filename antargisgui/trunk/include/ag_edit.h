#ifndef __AG_EDIT_H
#define __AG_EDIT_H

#include "ag_widget.h"
#include "ag_font.h"
#include "ag_gsurface.h"

class AGEditLine
{
 public:
  AGEditLine(const std::string &pText,AGFont pFont,bool pHardEnd);
  virtual ~AGEditLine();

  virtual void draw(const AGPoint &pPoint,const AGRect &pClip);
  virtual void drawCursor(int cx,const AGPoint &pPoint,const AGRect &pClip,const AGColor &c);
  
  int lines() const;

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
  void setText(const std::string s);

  bool hardEnd() const;
  void setHardEnd(bool pHardEnd);
  std::pair<std::string,bool> AGEditLine::checkUnwrap(int pW,std::string s);

  std::pair<std::string,bool> AGEditLine::checkWrap(int pW);

 private:
  std::string mText;
  AGFont mFont;
  bool mHardEnd;
};

class AGEdit:public AGWidget
{
 public:
  AGEdit(AGWidget *pParent,const AGRect &pRect);

  virtual void draw(const AGRect &pRect);
  virtual void drawBackground(const AGRect &pRect);

  virtual bool eventKeyDown(const AGEvent *m);
  virtual bool eventKeyUp(const AGEvent *m);

  void setMulti(bool mMultiLine);

  void insert(char c);
  void doDelete(int p);
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

  AGTexture mBackground;
};


#endif
