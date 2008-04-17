#include "ant_example_triangle.h"
#include "ant_gl.h"

void AntExampleTriangle::render(const AntRenderMode &Mode)
  {

    //glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             // Clear The Screen And The Depth Buffer
    //glLoadIdentity();                                               // Reset The View
    //glTranslatef(-1.5f,0.0f,-6.0f);                                 // Move Left 1.5 Units And Into The Screen 6.0
    
    glBegin(GL_TRIANGLES);                                              // Drawing Using Triangles
    glVertex3f( 0.0f, 1.0f, 0.0f);                          // Top
    glVertex3f(-1.0f,-1.0f, 0.0f);                          // Bottom Left
    glVertex3f( 1.0f,-1.0f, 0.0f);                          // Bottom Right
    glEnd();        
    //glPopMatrix();

  }
