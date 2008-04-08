
#include "ant_render_scene_pipelet.h"

AntRenderScenePipelet::AntRenderScenePipelet(AntRenderTarget *pRenderTarget,AntCameraView *pView,AntRenderMode pMode):
  AntRenderPipelet(pRenderTarget),
  mView(pView),
  mMode(pMode)
  {
    
  }
AntRenderScenePipelet::~AntRenderScenePipelet()
  {
    
  }

void AntRenderScenePipelet::render()
  {
    mView->render(mMode);
  }
AntCameraView *AntRenderScenePipelet::getView()
  {
    return mView;
  }

