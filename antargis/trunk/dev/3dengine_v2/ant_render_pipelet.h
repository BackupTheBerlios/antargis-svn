#ifndef ANT_RENDER_PIPELET_H
#define ANT_RENDER_PIPELET_H

#include <set>

#include "ant_render_target.h"

class AntRenderPipelet
  {
    std::set<AntRenderPipelet*> mPrerequisites;
    AntRenderTarget *mRenderTarget;
public:
  AntRenderPipelet(AntRenderTarget *pRenderTarget);
  virtual ~AntRenderPipelet();

  virtual void render();
  
  void doRender();

  std::set<AntRenderPipelet*> getPrerequisites() const;

  void addPrerequisite(AntRenderPipelet *pPipelet);

  };

#endif
