#ifndef __RENDERER_H
#define __RENDERER_H

#include "glee/GLee.h"

class Scene;

class Renderer
{
 public:
  Renderer();
  ~Renderer();

  bool canMultitexture();
  bool canShadow();
  bool canGLSL();

  GLint getShadowUnit();
  GLint getNormalUnit();

  void setCurrentScene(Scene *scene);
  Scene *getCurrentScene();

  void initShadowTexture();
  void beginShadowComputation();
  void endShadowComputation();

  void beginShadowDrawing();
  void endShadowDrawing();

  bool badShadowMap();
  
 private:

  
  int mCanMultitexture;
  int mCanShadow;
  int mCanGLSL;

  GLuint shadowMapTexture;
  int shadowMapSize;
  bool shadowInited;

  Scene *mScene;
};

Renderer *getRenderer();

#endif

