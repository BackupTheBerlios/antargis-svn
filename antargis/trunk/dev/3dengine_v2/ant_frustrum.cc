#include "ant_frustrum.h"
#include "ant_gl.h"
#include <ag_debug.h>

AntFrustrum::AntFrustrum(float openAngle,float aspect,float pNear,float pFar)
  {
    GLint mode;
    glGetIntegerv(GL_MATRIX_MODE,&mode);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    gluPerspective(openAngle,aspect,pNear,pFar);
    float matrix[16];
    glGetFloatv(GL_PROJECTION,matrix);

    for(int x=0;x<4;x++)
      for(int y=0;y<4;y++)
        mFrustrum.set(y,x,matrix[x+y*4]); // FIXME: correct ???

    glPopMatrix();
    glMatrixMode(mode);
  }

void AntFrustrum::select()
  {
    CTRACE;
    GLint mode;
    glGetIntegerv(GL_MATRIX_MODE,&mode);
    glMatrixMode(GL_PROJECTION);
  //  glLoadMatrixf(mFrustrum);

    gluPerspective(45.0,800/600.0,0.1,100);

    glMatrixMode(mode);
    cdebug("MODE:"<<mode);
    cdebug("MODELVIEW:"<<GL_MODELVIEW);
    cdebug("GL_PROJECTION:"<<GL_PROJECTION);
  }
