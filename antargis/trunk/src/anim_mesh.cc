#include "anim_mesh.h"
#include "scene.h"

#include "ag_debug.h"

class Loader
{
  std::string s;
  size_t pos;
public:
  Loader(const std::string &p):s(p)
  {
    pos=0;
  }
  Loader &operator>>(Uint32 &i)
  {
    cdebug(pos);
    std::string p=s.substr(pos,4);
    i=*(Sint32*)(p.c_str());
    pos+=4;
    return *this;
  }
  Loader &operator>>(float &i)
  {
    cdebug(pos<<" "<<s.length());
    std::string p=s.substr(pos,4);
    i=*(float*)(p.c_str());
    pos+=4;
    return *this;
  }
};

AnimMeshData::AnimMeshData(const std::string &instr)
{
  Loader l(instr);

  Uint32 vs,ts;

  l>>vs;
  cdebug("vertices:"<<vs);
  for(Uint32 i=0;i<vs;i++)
    {
      cdebug("i:"<<i);
      float x,y,z,tx,ty,nx,ny,nz;
      Uint32 boneID;
      l>>x>>y>>z>>tx>>ty>>boneID>>nx>>ny>>nz;
      pos.push_back(AGVector3(x,y,z));
      uv.push_back(AGVector2(tx,ty));
      normal.push_back(AGVector3(nx,ny,nz));
      bone.push_back(boneID);
    }

  l>>ts;
  cdebug("triangles:"<<ts);
  Uint32 index;
  for(Uint32 i=0;i<ts*3;i++)
    {
      l>>index;
      indices.push_back(index);
    }
}


AnimMesh::AnimMesh(AnimMeshData *data):mData(data)
{
  curKey=0;
  mTime=0;
}

AnimMesh::~AnimMesh()
{
}

// at first try a simple animation without shaders
void AnimMesh::draw()
{
  glBegin(GL_TRIANGLES);
  
  // for a start do a simple drawing 
  for(std::vector<size_t>::iterator i=mData->indices.begin();i!=mData->indices.end();i++)
    {
      glNormal3fv(mData->normal[*i]);
      glTexCoord2fv(mData->uv[*i]);
      glVertex3fv(mData->pos[*i]);
    }

  glEnd();
}
