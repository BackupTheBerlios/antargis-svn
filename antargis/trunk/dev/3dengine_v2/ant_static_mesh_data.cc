#include "ant_static_mesh_data.h"
#include "ant_mesh_optimizer.h"

AntStaticMeshData::AntStaticMeshData()
  {
    mChangeLevel=0;
  }
AntStaticMeshData::~AntStaticMeshData()
  {

  }


size_t AntStaticMeshData::getChangeLevel() const
{
  return mChangeLevel;
}

void AntStaticMeshData::changed()
  {
    mChangeLevel++;
  }


void AntStaticMeshData::addVertex(AGVector4 pVertex, AGVector4 pColor, AGVector3 pNormal, AGVector2 pTex)
  {
    assert(mTexCoords3D.size()==0);
    mVertices.push_back(pVertex);
    mColors.push_back(pColor);
    mNormals.push_back(pNormal);
    mTexCoords2D.push_back(pTex);
    changed();
    if(mVertices.size()==1)
      mBBox=AGBox3(pVertex.dim3(),AGVector3(0,0,0));
    else
      mBBox.include(pVertex.dim3());

  }
void AntStaticMeshData::addVertex(AGVector4 pVertex, AGVector4 pColor, AGVector3 pNormal, AGVector3 pTex)
  {
    assert(mTexCoords2D.size()==0);
    mVertices.push_back(pVertex);
    mColors.push_back(pColor);
    mNormals.push_back(pNormal);
    mTexCoords3D.push_back(pTex);
    changed();
    if(mVertices.size()==1)
      mBBox=AGBox3(pVertex.dim3(),AGVector3(0,0,0));
    else
      mBBox.include(pVertex.dim3());

  }
void AntStaticMeshData::addTriangle(size_t p0,size_t p1,size_t p2)
  {
    assert(p0<mVertices.size());
    assert(p1<mVertices.size());
    assert(p2<mVertices.size());
    mIndices.push_back(p0);
    mIndices.push_back(p1);
    mIndices.push_back(p2);
    changed();

  }


size_t AntStaticMeshData::getTriangleCount() const
{
  return mIndices.size()/3;
}
size_t AntStaticMeshData::getIndexCount() const
{
  return mIndices.size();
}
size_t AntStaticMeshData::getVertexCount() const
{
  return mVertices.size();
}

size_t AntStaticMeshData::getColorsCount() const
{
  return mColors.size();
}
size_t AntStaticMeshData::getNormalsCount() const
{
  return mNormals.size();
}
size_t AntStaticMeshData::getTexCoords2DCount() const
{
  return mTexCoords2D.size();
}
size_t AntStaticMeshData::getTexCoords3DCount() const
{
  return mTexCoords3D.size();
}




size_t AntStaticMeshData::getIndex(size_t i)
  {
    return mIndices[i];
  }
AGVector4 AntStaticMeshData::getVertex(size_t i)
  {
    return mVertices[i];
  }
AGVector4 AntStaticMeshData::getColor(size_t i)
  {
    return mColors[i];
  }
AGVector3 AntStaticMeshData::getNormal(size_t i)
  {
    return mNormals[i];
  }
AGVector2 AntStaticMeshData::getTexCoord2D(size_t i)
  {
    return mTexCoords2D[i];
  }
AGVector3 AntStaticMeshData::getTexCoord3D(size_t i)
  {
    return mTexCoords3D[i];
  }


Uint16 *AntStaticMeshData::getIndices()
  {
    return &(mIndices[0]);
  }
float *AntStaticMeshData::getVertices()
  {
    return (float*)&(mVertices[0]);
  }
float *AntStaticMeshData::getColors()
  {
    return (float*)&(mColors[0]);
  }
float *AntStaticMeshData::getNormals()
  {
    return (float*)&(mNormals[0]);
  }
float *AntStaticMeshData::getTexCoords2D()
  {
    return (float*)&(mTexCoords2D[0]);
  }
float *AntStaticMeshData::getTexCoords3D()
  {
    return (float*)&(mTexCoords3D[0]);
  }

bool AntStaticMeshData::has2DTextures() const
{
  return mTexCoords2D.size()!=0;
}
bool AntStaticMeshData::has3DTextures() const
{
  return mTexCoords3D.size()!=0;
}


// FIXME: change, that BinaryFileOut is used!
void AntStaticMeshData::save(const AGString &pFilename)
  {
    FILE *f=fopen(pFilename.c_str(),"wb");

    size_t meshes=1;
    size_t vertices=3;
    size_t faces=getTriangleCount();

    // meshes
    fwrite(&meshes,sizeof(Uint16),1,f);

    // faces
    fwrite(&faces,sizeof(Uint16),1,f);
    for(size_t i=0;i<faces;i++)
      {
        fwrite(&vertices,sizeof(Uint16),1,f);
        for(size_t k=0;k<vertices;k++)
          {
            size_t j=getIndex(i*vertices+k);
            fwrite(getVertex(j),sizeof(float),3,f);
            fwrite(getNormal(j),sizeof(float),3,f);
            fwrite(getColor(j),sizeof(float),3,f);
            fwrite(getTexCoord2D(j),sizeof(float),2,f); //FIXME: 3D not supported yet !!!
          }
      }

    fclose(f);

  }


AntStaticMeshData *AntStaticMeshData::load(const AGString &pFilename,float zoom)
  {
    Uint16 faces,meshes,vertices;

    AGBox3 bBox;

    AntMeshVertex mVertices[4];
    AntMeshOptimizer opt;
    bool withTex=pFilename.find(".ant2")!=pFilename.npos;

    BinaryFileIn f(pFilename);

    f>>meshes;

    for(;meshes>0;meshes--)
      {
        f>>faces;

        for(Uint16 i=0;i<faces;i++)
          {
            f>>vertices;
            assert(vertices<=4);
            for(Uint16 j=0;j<vertices;j++)
              {
                AGVector3 v,n,c;
                f>>v;
                mVertices[j].v=AGVector4(v,1);
                f>>n;
                mVertices[j].n=n;
                f>>c; // load here first, as mVertices[.].c is a AGVector4
                mVertices[j].c=AGVector4(c,1);
                if(withTex)
                  f>>mVertices[j].t;

                mVertices[j].t[1]=1-mVertices[j].t[1];

                mVertices[j].v*=zoom;
                mVertices[j].v[3]=1;

                bBox.include(mVertices[j].v.dim3());
              }
            if(vertices==3)
              {
                opt.add(mVertices[0]);
                opt.add(mVertices[1]);
                opt.add(mVertices[2]);
              }
            else
              {
                opt.add(mVertices[0]);
                opt.add(mVertices[1]);
                opt.add(mVertices[2]);
                opt.add(mVertices[0]);
                opt.add(mVertices[2]);
                opt.add(mVertices[3]);
              }
          }
      }
    AntStaticMeshData *mArray=opt.getNewMesh();
    mArray->mBBox=bBox;
    return mArray;
  }
