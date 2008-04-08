#ifndef ANT_RENDER_TARGET_H
#define ANT_RENDER_TARGET_H

#include "ant_fbo.h"

class AntRenderTarget
  {
public:
  virtual ~AntRenderTarget();
  virtual void select()=0;
  };



class AntFBORenderTarget:public AntRenderTarget
  {
    AntFBO *mFBO;
public:
  AntFBORenderTarget(AntFBO &pFBO);
  ~AntFBORenderTarget();
  virtual void select(); 
  };

#endif
