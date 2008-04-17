#include "ant_rigid_transform.h"
#include "ant_gl.h"
#include "ag_debug.h"

AntRigidTransform::AntRigidTransform(const AGVector3&pPos,const AGVector3 &pTo,const AGVector3 &pUp)
  {
    CTRACE;
    GLint mode;
    glGetIntegerv(GL_MATRIX_MODE,&mode);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    gluLookAt(pPos[0],
        pPos[1],
        pPos[2],
        pTo[0],
        pTo[1],
        pTo[2],
        pUp[0],
        pUp[1],
        pUp[2]
        );

    float matrix[16];
    glGetFloatv(GL_MODELVIEW,matrix);

    for(int x=0;x<4;x++)
      for(int y=0;y<4;y++)
        mMatrix.set(y,x,matrix[x+y*4]); // FIXME: correct ???

    glPopMatrix();
    glMatrixMode(mode);

  }
void AntRigidTransform::setup()
  {
    CTRACE;
    GLint mode;
    glGetIntegerv(GL_MATRIX_MODE,&mode);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mMatrix);
    //glLoadIdentity();
    //gluLookAt(0,-10,10,0,0,0,0,1,0);

    glMatrixMode(mode);
    
  }

