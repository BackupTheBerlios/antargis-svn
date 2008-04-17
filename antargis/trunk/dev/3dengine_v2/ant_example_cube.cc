#include "ant_example_cube.h"
#include "ant_gl.h"

void AntExampleCube::render(const AntRenderMode &pMode)
{
//  glDisable(GL_DEPTH_TEST);
  //glEnable(GL_COLORS);
  glColor3f(1,0,0);
  glBegin(GL_QUADS);
  glVertex3f(-1,0,0);
  glVertex3f(1,0,0);
  glVertex3f(1,1,0);
  glVertex3f(-1,1,0);
  glEnd();
}
