#ifndef ANT_SCREEN_RENDER_TARGET_H
#define ANT_SCREEN_RENDER_TARGET_H

#include "ant_render_target.h"
#include "ag_geometry.h"

class AntScreenRenderTarget:public AntRenderTarget
  {
    AGRect2 mRect;
public:
  AntScreenRenderTarget(const AGRect2 &pRect);
  void select();
  };

#endif
