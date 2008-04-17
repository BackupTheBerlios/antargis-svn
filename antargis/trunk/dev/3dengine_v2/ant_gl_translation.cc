#include "ant_gl_translation.h"
#include "ant_gl.h"

AntGLTranslation::AntGLTranslation(const AGVector3 &pTranslation)
  {
    glPushMatrix();
    glTranslatef(pTranslation[0],pTranslation[1],pTranslation[2]);
  }
AntGLTranslation::~AntGLTranslation()
  {
    glPopMatrix();
  }
