#include "ant_renderable.h"
#include "ant_gl_translation.h"
#include "ant_gl_rotation.h"

AntRenderable::AntRenderable(const AGVector3 &pPosition,const AntQuaternion &pRotation):
  mPosition(pPosition),
  mRotation(pRotation)
  {

  }

void AntRenderable::setPosition(const AGVector3 &pPosition)
  {
    mPosition=pPosition;
  }
AGVector3 AntRenderable::getPosition() const
{
  return mPosition;
}
void AntRenderable::setRotation(const AntQuaternion &pRotation)
  {
    mRotation=pRotation;
  }
AntQuaternion AntRenderable::getRotation() const
{
  return mRotation;
}

void AntRenderable::render(const AntRenderMode &pMode)
  { 
    AntGLTranslation t(mPosition);
    AntGLRotation rot(mRotation);
    renderObjectSpace(pMode);
  }
void AntRenderable::renderObjectSpace(const AntRenderMode &pMode)
  {

  }

AGBox3 AntRenderable::getBBox() const
{
  return AGBox3();
}
