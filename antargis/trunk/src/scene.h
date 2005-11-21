#ifndef __SCENE_H
#define __SCENE_H

#include "glee/GLee.h"
#include "ag_triangle.h"

#include <SDL_opengl.h>

#include <vector>
#include <set>
#include <map>


#include "scenenode.h"

struct PickNode
{
  AGVector4 pos;
  SceneNode *node;
  float camDist;
  
  bool operator<(const PickNode &n) const;
};


struct Viewport
{
  GLint viewport[4];
#ifndef SWIG
  (operator GLint *)()
  {
    return viewport;
  }
#endif
};


class Scene:public RubyObject
{
 public:
  Scene(int w,int h);
  virtual ~Scene();

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

  PickResult pick(float x,float y,float w,float h);

  AGLine3 getLine(int x,int y);

  // Antargis-Map-extension
  void mapChanged();

  size_t getTriangles() const;

  AGVector3 getCameraDirTo(const AGVector3 &p) const;


  AGMatrix4 getFrustum();

  float width() const;
  float height() const;

  void mark();

 private:
  void init();
  void calcCameraView();
  void calcShadowMap();
  void drawScene();
  void drawShadow();
  void initScene();


  void pickDraw();
  PickResult privatePick(float x,float y,float w,float h);
  PickResult processHits (int hits, GLuint *buffer,float x,float y);

  Viewport getViewport();


  bool inited;

  bool orthoShadow;
  int mShadow;

  AGVector4 cameraPosition;
  AGVector4 scenePosition;
  AGVector4 lightPosition;
  
  AGMatrix4 lightViewMatrix,lightProjectionMatrix;
  AGMatrix4 cameraViewMatrix,cameraProjectionMatrix;

  AGMatrix4 cameraPickMatrix;
  
  typedef std::vector<SceneNode*> Nodes;
  typedef std::set<SceneNode*> NodeSet;
  Nodes mNodes;
  NodeSet mNodeSet;

  int windowWidth,windowHeight;
  int shadowMapSize;
  GLuint shadowMapTexture;

  AGVector4 white,black;

  size_t mTriangles;

  // picking vars
  std::map<GLuint,SceneNode*> pickNames;
  

};

#endif
