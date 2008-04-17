#ifndef ANT_RENDERABLE
#define ANT_RENDERABLE

#include "ag_geometry.h"

#include "ant_quaternion.h"
#include "ant_scene_node.h"

class AntRenderable:public AntSceneNode
  {
    AGVector3 mPosition;
    AntQuaternion mRotation;
public:
  AntRenderable(const AGVector3 &pPosition,const AntQuaternion &pRotation);

  void setPosition(const AGVector3 &pPosition);
  AGVector3 getPosition() const;
  void setRotation(const AntQuaternion &pRotation);
  AntQuaternion getRotation() const;

  virtual void render(const AntRenderMode &pMode); 
  virtual void renderObjectSpace(const AntRenderMode &pMode); 

  virtual AGBox3 getBBox() const;
  };

#endif
