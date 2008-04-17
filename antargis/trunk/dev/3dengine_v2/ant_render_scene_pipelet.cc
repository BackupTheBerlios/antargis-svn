
#include "ant_render_scene_pipelet.h"

AntRenderScenePipelet::AntRenderScenePipelet(AntRenderTarget *pRenderTarget,AntCameraView *pView,AntRenderMode pMode,AntLighting *pLighting):
  AntRenderPipelet(pRenderTarget),
  mView(pView),
  mMode(pMode),
  mLighting(pLighting)
  {
    
  }
AntRenderScenePipelet::~AntRenderScenePipelet()
  {
    
  }

void AntRenderScenePipelet::render()
  {
    mLighting->setup();
    mView->render(mMode);
  }
AntCameraView *AntRenderScenePipelet::getView()
  {
    return mView;
  }

