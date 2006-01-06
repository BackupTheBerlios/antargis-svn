#ifndef AG_GLWIDGET_H
#define AG_GLWIDGET_H

#include <ag_widget.h>
#include <ag_triangle.h>

class AGGLWidget:public AGWidget
{
 public:
  AGGLWidget(AGWidget *pParent,const AGRect &r);

  virtual void drawGL();
  
  virtual void drawAll(AGPainter &p);
  
  float getRatio() const;

  void setPerspective(float openAngle,float near,float far);

 private:
  void beginGL();
  void endGL();

  AGMatrix4 pMatrix,mMatrix;
};

#endif
