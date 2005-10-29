#ifndef _SCENE_NODE_H
#define _SCENE_NODE_H

#include <ag_triangle.h>
#include <ruby.h>

class SceneNode
{
 public:
  SceneNode()
    {
      mRubyObject=false;
    }
  virtual ~SceneNode()
    {
      mRubyObject=false;
    }
  virtual void drawShadow()
  {
  }
  virtual void drawDepth()
  {
  }
  virtual void draw()
  {
  }
  virtual void advance(float time)
  {
  }
  virtual size_t getTriangles() const
  {
    return 0;
  }
  virtual AGVector4 lineHit(const AGLine3 &pLine) const
  {
    AGVector4 mfalse(0,0,0,0);
    return mfalse;
  }

  virtual void sort(const AGVector4 &pCamera)
  {
  }
  virtual void mapChanged()
  {
  }

  bool operator==(const SceneNode &n) const
  {
    return this==&n;
  }

  virtual void setPos(const AGVector3&pPos)
  {
  }

  virtual bool transparent()
  {
    return false;
  }

  
  public: // must be public, so that swig can set these
#ifndef SWIG
  bool mRubyObject;
  VALUE mRUBY;
#endif
};

typedef SceneNode *SceneNodePtr;
#endif
