#include "ant_mesh_va_data.h"
#include "ant_gl.h"
#include "ag_debug.h"

AntMeshVAData::AntMeshVAData(AntStaticMeshData *pMeshData):
  mMeshData(pMeshData)
                  {
                    mMeshChangeLevel=0;
                    mDynamic=false;
                    mVertexBuffer=mColorBuffer=mNormalBuffer=mTexBuffer=mIndexBuffer=0;
                    assert(mMeshData);
                  }

void AntMeshVAData::render(const AntRenderMode &pMode)
  {
    checkForChanges();

    glEnableClientState(GL_VERTEX_ARRAY);


    cdebug("mVertexBufer:"<<mVertexBuffer);
    cdebug("mTexBuffer:"<<mTexBuffer);
    if(mVertexBuffer)
      {
        if(mNormalBuffer)
          {
            glEnableClientState(GL_NORMAL_ARRAY);
            glBindBuffer( GL_ARRAY_BUFFER, mNormalBuffer );
            glNormalPointer(GL_FLOAT, 0, 0);
          }
        // FIXME: Texture-unit should be defined somewhere !!!

        if(mTexBuffer)
          {
            glClientActiveTexture(GL_TEXTURE0);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glBindBuffer( GL_ARRAY_BUFFER, mTexBuffer);
            if(has3DTextures())
              glTexCoordPointer(3, GL_FLOAT, 0, 0);
            else
              glTexCoordPointer(2, GL_FLOAT, 0, 0);
          }


        //FIXME : NORMALS ???




        glBindBuffer( GL_ARRAY_BUFFER, mVertexBuffer);
        glVertexPointer(4, GL_FLOAT, 0, 0);

        if(mDynamic)
          {
            glDrawElements(GL_TRIANGLES, mMeshData->getIndexCount(), GL_UNSIGNED_SHORT,
                mMeshData->getIndices());
          }
        else
          {
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
            glDrawElements(GL_TRIANGLES, mMeshData->getIndexCount(), GL_UNSIGNED_SHORT,0);
          }

      }
    else
      {
        
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, mMeshData->getNormals());

        if(has3DTextures())
          {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            glTexCoordPointer(3, GL_FLOAT, 0, mMeshData->getTexCoords3D());
          }
        else if(has2DTextures())
          {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, mMeshData->getTexCoords2D());
          }
        glVertexPointer(4, GL_FLOAT, 0, mMeshData->getVertices());

        glDrawElements(GL_TRIANGLES, mMeshData->getIndexCount(), GL_UNSIGNED_SHORT,
            mMeshData->getIndices());
      }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  }

void AntMeshVAData::checkForChanges()
  {
    if(mMeshChangeLevel!=mMeshData->getChangeLevel())
      {
        // update
        createBuffers();

        mMeshChangeLevel!=mMeshData->getChangeLevel();
      }
  }

void AntMeshVAData::dropBuffers()
  {
    if(mVertexBuffer!=0)
      glDeleteBuffers( 1, &mVertexBuffer );
    if(mColorBuffer!=0)
      glDeleteBuffers( 1, &mColorBuffer );
    if(mNormalBuffer!=0)
      glDeleteBuffers( 1, &mNormalBuffer );
    if(mTexBuffer!=0)
      glDeleteBuffers( 1, &mTexBuffer );
    if(mIndexBuffer!=0)
      glDeleteBuffers( 1, &mIndexBuffer );
    mVertexBuffer=mColorBuffer=mNormalBuffer=mTexBuffer=mIndexBuffer=0;
  }

bool AntMeshVAData::has3DTextures() const
{
  return mMeshData->has3DTextures();
}
bool AntMeshVAData::has2DTextures() const
{
  return mMeshData->has2DTextures();
}

bool AntMeshVAData::vertexBuffersActive() const
{
  return true;
}

void AntMeshVAData::createBuffers()
  {
    dropBuffers();

    if(!vertexBuffersActive())
      return;

    glGenBuffers( 1, &mVertexBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, mVertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, mMeshData->getVertexCount()*sizeof(AGVector4), mMeshData->getVertices(), GL_STATIC_DRAW );

    if(mMeshData->getColorsCount()>0)
      {
        assert(mMeshData->getColorsCount()==mMeshData->getVertexCount());

        glGenBuffers( 1, &mColorBuffer );
        glBindBuffer( GL_ARRAY_BUFFER, mColorBuffer );
        glBufferData( GL_ARRAY_BUFFER, mMeshData->getColorsCount()*sizeof(AGVector4), mMeshData->getColors(), GL_STATIC_DRAW );
      }
    if(mMeshData->getNormalsCount()>0)
      {
        assert(mMeshData->getNormalsCount()==mMeshData->getVertexCount());

        glGenBuffers( 1, &mNormalBuffer );
        glBindBuffer( GL_ARRAY_BUFFER, mNormalBuffer );
        glBufferData( GL_ARRAY_BUFFER, mMeshData->getNormalsCount()*sizeof(AGVector3), mMeshData->getNormals(), GL_STATIC_DRAW );
      }
    if(mMeshData->getTexCoords3DCount()>0)
      {
        assert(mMeshData->getTexCoords3DCount()==mMeshData->getVertexCount());

        glGenBuffers( 1, &mTexBuffer );
        glBindBuffer( GL_ARRAY_BUFFER, mTexBuffer );
        glBufferData( GL_ARRAY_BUFFER, mMeshData->getTexCoords3DCount()*sizeof(AGVector3),mMeshData->getTexCoords3D(), GL_STATIC_DRAW );
      }
    else if(mMeshData->getTexCoords2DCount()>0)
      {
        assert(mMeshData->getTexCoords2DCount()==mMeshData->getVertexCount());

        glGenBuffers( 1, &mTexBuffer );
        glBindBuffer( GL_ARRAY_BUFFER, mTexBuffer );
        glBufferData( GL_ARRAY_BUFFER, mMeshData->getTexCoords2DCount()*sizeof(AGVector2),mMeshData->getTexCoords2D(), GL_STATIC_DRAW );
      }
    if(!mDynamic)
      {
        glGenBuffers( 1, &mIndexBuffer );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, mMeshData->getIndexCount()*sizeof(Uint16), mMeshData->getIndices(), GL_STATIC_DRAW );
      }

    glBindBuffer( GL_ARRAY_BUFFER, 0);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);

  }
