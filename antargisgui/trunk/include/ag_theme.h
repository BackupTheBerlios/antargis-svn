#ifndef AG_THEME_H
#define AG_THEME_H

#include "ag_font.h"
#include "ag_surface.h"

#include <map>

class AGTheme
{
 public:
  AGTheme();
  virtual ~AGTheme();

  AGFont getFont(const std::string &pName);
  AGColor getColor(const std::string &pName);

  void setFont(const std::string &pName,AGFont pFont);
  void setColor(const std::string &pName,AGColor pColor);

  int getInt(const std::string &pName);
  void setInt(const std::string &pName,int i);

  AGSurface getSurface(const std::string &pName);
  void setSurface(const std::string &pName,const AGSurface &pSurface);

  bool hasSurface(const std::string &pName) const;

 private:

  std::string trunk(std::string s);

  std::map<std::string,AGFont> mFonts;
  std::map<std::string,AGColor> mColors;
  std::map<std::string,AGSurface> mSurfaces;
  std::map<std::string,int> mInts;
};

AGTheme *getTheme();
void setTheme(const AGTheme &aTheme);

#endif
