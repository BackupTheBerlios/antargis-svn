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
  virtual void draw(const AGRect &r);

  void setText(const std::string &pText);
  std::string getText() const;

 private:
  std::string mText;
  AGFont mFont;
  bool mFixedSize;
};

#endif
