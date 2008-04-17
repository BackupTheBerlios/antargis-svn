#ifndef ANT_PERSPECTIVE_CAMERA_H
#define ANT_PERSPECTIVE_CAMERA_H

#include "ant_camera.h"
#include "ant_frustrum.h"
#include "ant_rigid_transform.h"

class AntPerspectiveCamera:public AntCamera
  {
    AntFrustrum mFrustrum;
    AntRigidTransform mTransform;
    
public:
  AntPerspectiveCamera(const AntFrustrum &pFrustrum,const AntRigidTransform &pTransform);
  
  virtual void setupView();

  };


#endif
