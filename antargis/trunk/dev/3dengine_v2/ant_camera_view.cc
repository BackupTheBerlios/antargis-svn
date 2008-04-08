#include "ant_camera_view.h"

AntCameraView::AntCameraView(AntSceneGraph *pScene,AntCamera *pCamera):
  mScene(pScene),
  mCamera(pCamera)
    {

    }

void AntCameraView::render(const AntRenderMode &pMode)
  {
    mScene->render(*mCamera,pMode);
  }
