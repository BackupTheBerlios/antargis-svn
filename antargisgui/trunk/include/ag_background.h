#ifndef AG_BACKGROUND_H
#define AG_BACKGROUND_H

#include <string>
#include "ag_geometry.h"
#include "ag_surface.h"
#include "ag_color.h"

class AGBackground
{
 public:
  AGBackground(std::string pThemeName="");

  void draw(const AGRect &pScreenRect) const;

 private:
  AGTexture mSurface;
  AGColor mColors[4];

  bool mSurfaceFlag;
};

#endif
