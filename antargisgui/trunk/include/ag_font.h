#ifndef __AG_FONT_H
#define __AG_FONT_H

#include "ag_color.h"

#include <string>

class AGFont
{
 public:

  enum Style {NORMAL,BOLD,UNDERLINE,ITALIC};

  AGFont();
  AGFont(const std::string &pFile,int size=14);//,int index=0);

  void setColor(const AGColor &pColor);
  AGColor getColor() const;

  void setAlpha(Uint8 pAlpha);
  Uint8 getAlpha() const;

  void setSize(Uint8 pSize);
  Uint8 getSize() const;

  void setStyle(const Style &s);
  Style getStyle() const;

  void setName(const std::string &s);
  std::string getName() const;


  int getWidth(const std::string &mText) const;
  int getHeight(const std::string &mText) const;
 private:
  Style mStyle;
  std::string mName;
  int mSize;
  Uint8 mAlpha;
  AGColor mColor;


  friend bool operator<(const AGFont&f1,const AGFont &f2);
};

bool operator<(const AGFont&f1,const AGFont &f2);

#endif
