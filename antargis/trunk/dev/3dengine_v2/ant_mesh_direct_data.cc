#include "ant_mesh_direct_data.h"
#include "ant_gl.h"
#include "ag_debug.h"

AntMeshDirectData::AntMeshDirectData(AntStaticMeshData *pMeshData):
  mMeshData(pMeshData)
        {
        }

void AntMeshDirectData::render(const AntRenderMode &pMode)
  {
    glColor3f(1,1,1);
    size_t i;
    glBegin(GL_TRIANGLES);
    /*glVertex3f(-1,0,0);
    glVertex3f(1,0,0);
    glVertex3f(1,1,0);
     */
    for(i=0;i<mMeshData->getIndexCount();i++)
      {
        AGVector4 v=mMeshData->getVertex(mMeshData->getIndex(i));
        AGVector3 n=mMeshData->getNormal(mMeshData->getIndex(i));
        cdebug("v:"<<v);
        glNormal3fv(n);
        glVertex4fv(v);
      }
    glEnd();
  }
