#ifndef ANT_APP_H
#define ANT_APP_H

#include "scene.h"
#include "renderer.h"
#include <iostream>
#include <fstream>
#include <ag_application.h>
#include <ag_debug.h>
#include <ag_main.h>
#include <ag_geometry.h>

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

  virtual bool eventMouseButtonDown(AGEvent *m);
  virtual bool eventMouseButtonUp(AGEvent *m);
  virtual bool eventMouseMotion(AGEvent *m);
  virtual bool eventKeyDown(AGEvent *m);

  Scene &getScene();


  virtual void setCamera(const AGVector2&p);
  AGVector2 getCamera() const;

  void mark();
};



#endif
