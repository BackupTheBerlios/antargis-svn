#ifndef AG_CAPTION_H
#define AG_CAPTION_H

#include "ag_text.h"
#include "ag_background.h"

class AGCaption:public AGText
{
 public:
  AGCaption(AGWidget *pParent,const AGRect &pRect,const std::string &pText,const AGFont &pFont,const AGBackground &pBG);

  void draw(const AGRect &pRect);

  void setBackground(AGBackground pBG);
 private:
  AGBackground mBG;
};

#endif
