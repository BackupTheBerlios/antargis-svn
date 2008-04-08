#ifndef ANT_RENDER_PIPELINE_H
#define ANT_RENDER_PIPELINE_H

#include <set>
#include <list>

#include "ant_render_pipelet.h"
#include "ant_camera_view.h"
#include "ant_render_mode.h"

class AntRenderScenePipelet:public AntRenderPipelet
  {
    std::set<AntRenderPipelet*> mPrerequisites;
    AntCameraView *mView;
    AntRenderMode mMode;
public:
  AntRenderScenePipelet(AntRenderTarget *pRenderTarget,AntCameraView *pView,AntRenderMode pMode);
  ~AntRenderScenePipelet();

  void render();

protected:
  AntCameraView *getView();

  };

#endif

