#ifndef __SCENE_H
#define __SCENE_H

//#include "vector.h"
//#include "matrix.h"
#include "ag_triangle.h"

#include <SDL_opengl.h>

#include <vector>
#include <set>


#include "scenenode.h"

struct PickNode
{
  AGVector4 pos;
  SceneNode *node;
  float camDist;
  
  bool operator<(const PickNode &n) const;
};


class Scene
{
 public:
  Scene(int w,int h);

  void draw();

  // 0 == none, 1 == shadow mapping, 2 == perspective shadow mapping
  void setShadow(int v);
  int getShadow() const;

  void addNode(SceneNode *node);
  void removeNode(SceneNode *node);

  void clear();

  // (mx,my,0)
  void setCamera(AGVector4 v);
  void advance(float time);

  typedef std::vector<PickNode> PickResult;
  
  PickResult lineHit(const AGLine3 &pLine);

  AGLine3 getLine(int x,int y);

  // Antargis-Map-extension
  void mapChanged();

  AGVector3 getCameraDirTo(const AGVector3 &p) const;


 private:
  void init();
  void calcCameraView();
  void calcShadowMap();
  void drawScene();
  void drawShadow();
  void initScene();

  bool inited;

  bool orthoShadow;
  int mShadow;

  AGVector4 cameraPosition;
  AGVector4 scenePosition;
  AGVector4 lightPosition;
  
  AGMatrix4 lightViewMatrix,lightProjectionMatrix;
  AGMatrix4 cameraViewMatrix,cameraProjectionMatrix;
  
  typedef std::vector<SceneNode*> Nodes;
  typedef std::set<SceneNode*> NodeSet;
  Nodes mNodes;
  NodeSet mNodeSet;

  int windowWidth,windowHeight;
  int shadowMapSize;
  GLuint shadowMapTexture;

  AGVector4 white,black;

  size_t mTriangles;

  friend void Scene_markfunc(void *ptr);

  public: // must be public, so that swig can set these
  bool mRubyObject;
  VALUE mRUBY;
};

#endif
