#include "ant_lighting.h"
#include "ant_gl.h"

#include "ag_geometry.h"

void AntLighting::setup()
  {
    //return;
    AGVector4 l(5,-10,10,1);

    AGVector4 white=AGVector4(1,1,1,1);
    AGVector4 black=AGVector4(0,0,0,1);
    
    glLightfv(GL_LIGHT1, GL_POSITION, l);
    glLightfv(GL_LIGHT1, GL_AMBIENT, AGVector4(0.1,0.1,0.1,1));//white*0.05f);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, AGVector4(0.3,0.3,0.3,1));//white*0.3f);//*0.2f);
    glLightfv(GL_LIGHT1, GL_SPECULAR, black);
    glEnable(GL_LIGHT1);

    AGVector4 lightPosition2=AGVector4( 0, 0, -50,1);

    //  glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2+scenePosition);
    glLightfv(GL_LIGHT2, GL_AMBIENT, black);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, AGVector4(0.7,0.7,0.7,1));//white*0.7f);//*0.2f);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, AGVector4(0,0,-1,0));
    glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,0);
    glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,0);
    glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION,0);
    glEnable(GL_LIGHT2);



    glEnable(GL_LIGHTING);

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);

  }
