#include "ag_glwidget.h"

AGGLWidget::AGGLWidget(AGWidget *pParent,const AGRect &r):
  AGWidget(pParent,r)
{
}

void AGGLWidget::drawGL()
{
  
}
  
void AGGLWidget::drawAll(AGPainter &p)
{
  beginGL();
  drawGL();
  endGL();
}

void AGGLWidget::beginGL()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  setPerspective(45.0,1,100);
  glMatrixMode(GL_MODELVIEW);

  AGRect r=getRect();

  glViewport(r.x,getScreen().getHeight()-(r.y+r.h),r.x+r.w,getScreen().getHeight()-r.y);
  glDepthMask(false);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  
  
}

float AGGLWidget::getRatio() const
{
  AGRect r=getRect();
  return float(r.w)/float(r.h);
}


void AGGLWidget::endGL()
{
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);

  glEnable(GL_DEPTH_TEST); // enable depth test
  glDepthFunc(GL_LEQUAL); // set type depth test
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // GL_NICEST // best perspective correction
  glEnable(GL_BLEND);

  glViewport( 0, 0, getScreen().getWidth(), getScreen().getHeight() );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  gluOrtho2D(0,getScreen().getWidth(),0,getScreen().getHeight());

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );


  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glDisable(GL_DEPTH_TEST); // enable depth test

  glDepthMask(false);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//NEAREST);//LINEAR);
  glEnable(GL_COLOR_MATERIAL);
}

void AGGLWidget::setPerspective(float openAngle,float near,float far)
{
  glMatrixMode(GL_PROJECTION);
  gluPerspective(openAngle,getRatio(),near,far);
  glGetFloatv(GL_PROJECTION_MATRIX, pMatrix);
}
