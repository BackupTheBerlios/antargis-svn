#ifndef ANT_WORLD
#define ANT_WORLD

#include <set>

#include "ant_camera_view.h"

class AntWorld
  {
    AntSceneGraph *mSceneGraph;
    std::set<AntCamera> *mCameras;
    std::set<AntCameraViews> *mViews;
    std::set<AntRenderPipeline> *mPipelines;
public:
  void render();
  
  AntCamera *newCamera();
  AntCamerView *newView(const AntCamera &pCamera);
  
  
  
  };

#endif
