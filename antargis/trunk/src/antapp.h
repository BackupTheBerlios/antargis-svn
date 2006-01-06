#ifndef ANT_APP_H
#define ANT_APP_H

#include "scene.h"
#include "renderer.h"
#include <iostream>
#include <fstream>
#include <ag_application.h>
#include <ag_debug.h>
#include <ag_main.h>
#include <ag_triangle.h>

#ifndef SWIG
extern std::ofstream out;
#endif

class GLApp:public AGApplication
{
  bool shadow;
  int mx,my;
  int omx,omy;
  AGVector4 camera;
  float frameTime;
  size_t frameCount;

  bool mMayClick;

  int hx,hy,hb;
  Renderer renderer;

protected:
  Scene scene;
  
public:
  GLApp(int w,int h);
  ~GLApp();

  virtual void draw();
  void drawGL();

  bool eventFrame(float t);

  virtual void eventClick(const Scene::PickResult &pNodes,int button);
  virtual void eventHover(const Scene::PickResult &pNodes,int button);

  virtual bool eventMouseButtonDown(const AGEvent *m);
  virtual bool eventMouseButtonUp(const AGEvent *m);
  virtual bool eventMouseMotion(const AGEvent *m);
  virtual bool eventKeyDown(const AGEvent *m);

  Scene &getScene();

  void mark();
};



#endif
