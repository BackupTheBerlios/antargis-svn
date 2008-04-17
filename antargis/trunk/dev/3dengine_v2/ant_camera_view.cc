#include "ant_camera_view.h"
#include <ag_debug.h>

AntCameraView::AntCameraView(AntSceneGraph *pScene,AntCamera *pCamera):
  mScene(pScene),
  mCamera(pCamera)
    {

    }

void AntCameraView::render(const AntRenderMode &pMode)
  {
    CTRACE;
    mCamera->setupView();
    mScene->render(*mCamera,pMode);
  }
