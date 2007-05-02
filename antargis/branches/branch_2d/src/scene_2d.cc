#ifndef NO

#include "scene_2d.h"
#include <ag_debug.h>

Scene2D::Scene2D(int w,int h):
  SceneBase(w,h),mPainter(0)
{
}

Scene2D::~Scene2D()
{
}

void Scene2D::draw()
{
  // FIXME
  
  NodeList nodeList=getCurrentNodes();

  //FIXME:sort!!

  for(NodeList::iterator i=nodeList.begin();i!=nodeList.end();i++)
    (*i)->draw();
}

SceneBase::PickResult Scene2D::pick(float x,float y,float w,float h)
{
  throw std::runtime_error("FIXME");
  return PickResult();
}

AGVector2 Scene2D::getPosition(const AGVector4 &v) const
{
  // FIXME: include camera !!
  AGVector2 center(width()/2,height()/2);
  AGVector2 cam(mCamera.getPosition().dim2());

  AGVector2 n=v.dim2()+center-cam;

  //  cdebug("n:"<<n);

  return AGVector2(n[0],height()-n[1]);

  throw std::runtime_error("FIXME");
  return v.dim2();
}

SceneBase::NodeList Scene2D::getCurrentNodes()
{
  //  cdebug("FIXME");

  NodeList l;
  std::copy(mNodes.begin(),mNodes.end(),std::back_inserter(l));

  //  throw std::runtime_error("FIXME");
  return l;
}

//void setEnabled(bool p);

void Scene2D::setPainter(AGPainter *p)
{
  CTRACE;
  assert(mPainter==0);
  mPainter=p;
}
void Scene2D::discardPainter()
{
  CTRACE;
  assert(mPainter!=0);
  mPainter=0;
}

AGPainter *Scene2D::getPainter()
{
  CTRACE;
  assert(mPainter);
  return mPainter;
}


#endif
