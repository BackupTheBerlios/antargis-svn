#ifndef ANT_CAMERA_VIEW_H
#define ANT_CAMERA_VIEW_H

#include "ant_camera.h"
#include "ant_scene_graph.h"

class AntCameraView
  {
    AntSceneGraph *mScene;
    AntCamera *mCamera;
public:
  AntCameraView(AntSceneGraph *pScene,AntCamera *pCamera);
  
  void render(const AntRenderMode &pMode);
  };

#endif
