#ifndef __SCENE_H
#define __SCENE_H

#include "GLee.h"
#include "ag_geometry.h"
#include "ag_geometry.h"

#include <SDL_opengl.h>

#include <vector>
#include <set>
#include <map>

#include "scenenode.h"
#include "ant_camera.h"

struct PickNode
{
  AGVector4 pos;
  SceneNode *node;
  float camDist;
  
  bool operator<(const PickNode &n) const;
};

/*
struct Viewport
{
  GLint viewport[4];
#ifndef SWIG
  (operator GLint *)()
  {
    return viewport;
  }
#endif
};*/

template<class T>
class QuadTree;

class Scene:public AGRubyObject
{
 public:
  typedef std::vector<PickNode> PickResult;
  typedef std::list<SceneNode*> NodeList;

  Scene(int w,int h);
  virtual ~Scene();

  void draw();

  // 0 == none, 1 == shadow mapping, 2 == perspective shadow mapping
  void setShadow(int v);
  int getShadow() const;


  // ATTENTION: nodes are not owned by Scene afterwards - so they won't get deleted!
  //            You have to do this yourself in the Entities or let ruby's GC do it for you 
  void addNode(SceneNode *node);
  void removeNode(SceneNode *node);
  void prepareUpdate(SceneNode *node);
  void updatePos(SceneNode *node);


  void clear();

  // (mx,my,0)
  void setCamera(AGVector4 v);
  AGVector4 getCamera() const;
  void advance(float time);

  PickResult pick(float x,float y,float w,float h);

  // Antargis-Map-extension
  //  void mapChanged();

  AntCamera &getCameraObject();

  size_t getTriangles() const;

  AGVector3 getCameraDirTo(const AGVector3 &p) const;

  AGMatrix4 getFrustum();

  float width() const;
  float height() const;

  void mark();

  AGMatrix4 getLightComplete() const;
  AGMatrix4 getLightView() const;
  AGMatrix4 getLightProj() const;
  
  //  AGMatrix4 getInvCameraView() const;

  AGVector2 getPosition(const AGVector4 &v) const;

  NodeList getCurrentNodes();


  void setEnabled(bool p);

 private:
  void calcShadowMap();
  void drawScene();
  void drawShadow();
  void initScene();

  void pickDraw();
  PickResult processHits (int hits, GLuint *buffer,float x,float y);

  Viewport getViewport() const;

  int mShadow;

  AGMatrix4 cameraPickMatrix;
  
  typedef std::vector<SceneNode*> Nodes;
  typedef std::set<SceneNode*> NodeSet;

  typedef QuadTree<SceneNode> Tree;

  Tree *mTree;

  AntCamera mCamera;

  Nodes mNodes;
  NodeSet mNodeSet;

  AGVector4 white,black;

  size_t mTriangles;

  // picking vars
  std::map<GLuint,SceneNode*> pickNames;

  bool mEnabled;
};


#endif
