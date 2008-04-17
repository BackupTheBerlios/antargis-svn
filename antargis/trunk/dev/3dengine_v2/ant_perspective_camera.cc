#include "ant_perspective_camera.h"

AntPerspectiveCamera::AntPerspectiveCamera(const AntFrustrum &pFrustrum,const AntRigidTransform &pTransform):
  mFrustrum(pFrustrum),
  mTransform(pTransform)
  {
    
  }

void AntPerspectiveCamera::setupView()
  {
    mFrustrum.select();
    mTransform.setup();
  }
