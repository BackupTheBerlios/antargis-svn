#ifndef AG_FONTENGINE_H
#define AG_FONTENGINE_H

#include <string>

#include "ag_surface.h"
#include "ag_font.h"
#include "ag_geometry.h"

class AGFontEngine
{
 public:
  AGFontEngine();
  ~AGFontEngine();


  //  static bool renderText (AGSurface &pSurface, const AGRect &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn);
  static bool renderText (AGGScreen *pScreen, const AGRect &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn);

  static int getWidth(const AGFont &pFont,const std::string &pText);
  static int getHeight(const AGFont &pFont,const std::string &pText);
};

#endif

