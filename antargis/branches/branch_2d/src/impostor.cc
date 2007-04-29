#include "impostor.h"
#include <ag_debug.h>
#include <ag_screen.h>
#include <ag_rendercontext.h>

AntImpostorData::AntImpostorData(SceneNode *pNode):mTexture(512,512)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear screen and depth buffer

  mTexture.beginPaint();
  glEnable(GL_DEPTH_TEST); // enable depth test
  glDepthMask(true);

  assertGL;

  AGRenderContext c;
  c.begin(); // reset gl-state


  assert(pNode);
  Scene *pScene=dynamic_cast<Scene*>(pNode->getScene());
  assert(pScene);

  pScene->initScene();

  /*
  AntCamera camera=pScene->getCameraObject();


  // init smaller Projection-matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  int x=0,y=0,h=mTexture.width(),w=mTexture.height();

  gluPickMatrix(x,camera.getHeight()-y,h,w,pScene->getViewport());

  // add cameras-projection-matrix
  glMultMatrixf(camera.getProjection());

  glMatrixMode(GL_MODELVIEW);
  */
  int sw=getScreen().getWidth(),sh=getScreen().getHeight();
  int w=mTexture.width(),h=mTexture.height();
  
  

  glViewport(-sw/2+w/2,-sh/2+h/2,sw,sh);

  pNode->draw();



  //  mTexture.endPaint();

  AGSurface s=getScreen().screenshot(false);
  s.save("impostor.png");


  mTexture.endPaint();

  AGSurface subs=s.getSubSurface(AGRect2(0,sh-h,w,h));
  subs.save("impostor3.png");

  //  mTexture.save("impostor.png");
}

AGTexture &AntImpostorData::getTexture()
{
  return mTexture;
}

void AntImpostorData::mark()
{
  markObject(&mTexture);
  AGRubyObject::mark();
}



void AntImpostorData::create()
{
}


/////////////////////////////////////////////////////////////////
// AntImpostor
/////////////////////////////////////////////////////////////////

AntImpostor::AntImpostor(Scene *pScene,AntImpostorData *pData,const AGVector4 &pPos,float pRot):
  SceneNode(pScene,pPos,AGBox3())
{
}

void AntImpostor::draw()
{
}
void AntImpostor::drawDepth()
{
}
void AntImpostor::drawShadow()
{
}
void AntImpostor::drawPick()
{
}

