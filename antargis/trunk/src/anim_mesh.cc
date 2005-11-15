#include "anim_mesh.h"
#include "scene.h"
#include "ag_texturecache.h"

#include "ag_debug.h"

#include <math.h>

#define USE_ANIM

class Loader
{
  std::string s;
  size_t pos;
public:
  Loader(const std::string &p):s(p)
  {
    pos=0;
  }
  Loader &operator>>(Sint32 &i)
  {
    std::string p=s.substr(pos,4);
    i=*(Sint32*)(p.c_str());
    pos+=4;
    return *this;
  }
  Loader &operator>>(Uint32 &i)
  {
    std::string p=s.substr(pos,4);
    i=*(Sint32*)(p.c_str());
    pos+=4;
    return *this;
  }
  Loader &operator>>(float &i)
  {
    std::string p=s.substr(pos,4);
    i=*(float*)(p.c_str());
    pos+=4;
    return *this;
  }
};

void setRotation(AGMatrix4 &m,const AGVector3 &angles)
{
  double cr = cos( angles[0] );
  double sr = sin( angles[0] );
  double cp = cos( angles[1] );
  double sp = sin( angles[1] );
  double cy = cos( angles[2] );
  double sy = sin( angles[2] );
  // FIXME: maybe the coord must be switched

  m.set(0,0,( float )( cp*cy ));
  m.set(0,1,( float )( cp*sy ));
  m.set(0,2,( float )( -sp ));
  
  double srsp = sr*sp;
  double crsp = cr*sp;

  m.set(1,0,( float )( srsp*cy-cr*sy ));
  m.set(1,1,( float )( srsp*sy+cr*cy ));
  m.set(1,2,( float )( sr*cp ));
  
  m.set(2,0,( float )( crsp*cy+sr*sy ));
  m.set(2,1,( float )( crsp*sy-sr*cy ));
  m.set(2,2,( float )( cr*cp ));

}

void setTranslation(AGMatrix4 &m,const AGVector3 &t)
{
  m.set(3,0,t[0]);
  m.set(3,1,t[1]);
  m.set(3,2,t[2]);
}


void inverseRotate(AGVector3 &v, const AGMatrix4 &m )
{
  AGVector3 n;
  n[0]=v[0]*m.get(0,0) + v[1]*m.get(0,1) + v[2]*m.get(0,2);
  n[1]=v[0]*m.get(1,0) + v[1]*m.get(1,1) + v[2]*m.get(1,2);
  n[2]=v[0]*m.get(2,0) + v[1]*m.get(2,1) + v[2]*m.get(2,2);
  v=n;
}

void inverseTranslate(AGVector3 &v, const AGMatrix4 &m )
{
  v[0]-=m.get(3,0);
  v[1]-=m.get(3,1);
  v[2]-=m.get(3,2);
}


AnimMeshData::AnimMeshData(const std::string &instr,float scale,const std::string &tex)
{
  mTexture=getTextureCache()->get(tex);
  Loader l(instr);

  Uint32 vs,ts;

  l>>vs;
  for(Uint32 i=0;i<vs;i++)
    {
      float x,y,z,tx,ty,nx,ny,nz;
      Sint32 boneID;
      l>>x>>y>>z>>tx>>ty>>boneID>>nx>>ny>>nz;
      pos.push_back(AGVector3(x,y,z)*scale);
      uv.push_back(AGVector2(tx,ty));
      normal.push_back(AGVector3(nx,ny,nz));
      bone.push_back(boneID);
    }

  l>>ts;
  Uint32 index;
  for(Uint32 i=0;i<ts*3;i++)
    {
      l>>index;
      indices.push_back(index);
    }

  Uint32 nbones;
  l>>nbones;

  for(Uint32 i=0;i<nbones;i++)
    {
      float x,y,z,rx,ry,rz;
      Sint32 parent;
      l>>x>>y>>z>>rx>>ry>>rz>>parent;
      Bone *bone=new Bone;
      bone->id=i;
      bone->pos=AGVector3(x,y,z)*scale;
      bone->rot=AGVector3(rx,ry,rz);
      if(parent<0)
	bone->parent=0;
      else
	{
	  assert(parent<(int)i);
	  bone->parent=bones[parent];
	}
      bones.push_back(bone);
    }

  // read key frames

  Uint32 nframes;
  l>>nframes;
  animTime=nframes;

  Uint32 keys;
  l>>keys;

  for(Uint32 i=0;i<keys;i++)
    {
      float time;
      l>>time;
      KeyFrame *frame=new KeyFrame;
      frame->time=time-1;
      for(Uint32 j=0;j<nbones;j++)
	{
	  float x,y,z,rx,ry,rz;
	  l>>x>>y>>z>>rx>>ry>>rz;
	  
	  frame->pos.push_back(AGVector3(x,y,z)*scale);
	  frame->rot.push_back(AGVector3(rx,ry,rz));

	  frames.push_back(frame);
	}
    }

#ifdef USE_ANIM
  setupJoints();
#endif
}

void AnimMeshData::setTransform(const AGMatrix4 &m)
{
  mBase=m;
}

const AGMatrix4 &AnimMeshData::getTransform() const
{
  return mBase;
}


void AnimMeshData::setupJoints()
{
  for (std::vector<Bone*>::iterator i = bones.begin(); i!=bones.end(); i++ )
    {
      Bone& joint = **i;

      setRotation(joint.mRelative, joint.rot);
      setTranslation(joint.mRelative, joint.pos);

      if(joint.parent)
	joint.mAbsolute=joint.parent->mAbsolute*joint.mRelative;
      else
	joint.mAbsolute=joint.mRelative;

    }

  for (size_t i=0;i<pos.size();i++)
    {
      if(bone[i]!=-1)
	{
	  AGMatrix4 m(bones[bone[i]]->mAbsolute);
	  AGVector3 test=pos[i];
	  inverseTranslate(pos[i],m);
	  inverseRotate(pos[i],m);

	  // normals
	  inverseRotate(normal[i],m);
	}
    }
}




/////////////////////////////////////////////////////////////////////////
// AnimMesh
/////////////////////////////////////////////////////////////////////////




AnimMesh::AnimMesh(AnimMeshData *data):mData(data)
{
  curKey=0;
  mTime=0;
}

AnimMesh::~AnimMesh()
{
  CTRACE;
}



// at first try a simple animation without shaders
void AnimMesh::draw()
{
  glColor4f(1,1,1,1);
  glBindTexture(GL_TEXTURE_2D,mData->mTexture.getTextureID());
  glEnable(GL_LIGHTING);

  glPushMatrix();
  glTranslatef(mPos[0],mPos[1],mPos[2]);
  glRotatef(mRot[3],mRot[0],mRot[1],mRot[2]);

  #ifndef USE_ANIM
    {
      glBegin(GL_TRIANGLES);
      
      // for a start do a simple drawing 
      for(std::vector<size_t>::iterator i=mData->indices.begin();i!=mData->indices.end();i++)
	{
	  AGMatrix4 m(mData->getTransform());
	  AGVector3 n((m*AGVector4(mData->normal[*i],0)).dim3());
	  AGVector3 p((m*AGVector4(mData->pos[*i],1)).dim3());
				      
	  glNormal3fv(n);
	  glTexCoord2fv(mData->uv[*i]);
	  glVertex3fv(p);
	  
	  /*
	    glNormal3fv(mData->normal[*i]);
	    glTexCoord2fv(mData->uv[*i]);
	    glVertex3fv(mData->pos[*i]);*/
	}
      
      glEnd();
    }
#else
    {

      //      cdebug(mTime);
      // calculate 
      std::vector<AGMatrix4> ms;

      while(mTime>mData->animTime)
	mTime-=mData->animTime;

      // first get two key frames
      KeyFrame *f0,*f1;

      //      cdebug(mTime);
      std::vector<KeyFrame*>::iterator i=mData->frames.begin();
      //      cdebug((*i)->time);
      while(i!=mData->frames.end() && (*i)->time<=mTime)
	i++;
      if(i==mData->frames.end())
	i--;
      //      cdebug((*i)->time);
      f1=*i;
      //      assert(i!=mData->frames.begin());
      if(i!=mData->frames.begin())
	i--;
      f0=*i;
      
      // calc matrices for bones

      
      float interpol;
      if(f0->time>f1->time)
	{
	  if(mTime<f0->time)
	    mTime+=mData->animTime;
	  interpol=(mTime-f0->time)/(f1->time+mData->animTime);
	}
      else if(f0->time==f1->time)
	interpol=0;
      else
	interpol=(mTime-f0->time)/(f1->time-f0->time);
      

      assert(interpol>=0.0f);
      assert(interpol<=1.0f);
      float i2=1.0f-interpol;

      for(size_t k=0;k<mData->bones.size();k++)
	{
	  AGMatrix4 final(mData->bones[k]->mRelative);
	  AGMatrix4 trans;
	  AGVector3 rot,pos;

	  rot=f0->rot[k]*i2 + f1->rot[k]*interpol;
	  pos=f0->pos[k]*i2 + f1->pos[k]*interpol;

	  ::setRotation(trans,rot);
	  setTranslation(trans,pos);

		   
	  final=final*trans;

	  if(mData->bones[k]->parent)
	    final=mData->bones[k]->parent->mFinal*final;
	  //	  else
	  //	    final*=mData->getTransform();

	  mData->bones[k]->mFinal=final;
	}
      

      // paint with transform

      glPushMatrix();
      glMultMatrixf(mData->getTransform());

      //      glScalef(0.1,0.1,0.1);
      glBegin(GL_TRIANGLES);
      
      // for a start do a simple drawing 
      for(std::vector<size_t>::iterator i=mData->indices.begin();i!=mData->indices.end();i++)
	{
	  int b=mData->bone[*i];
	  AGMatrix4 m;
	  if(b>=0)
	    {
	      m=mData->bones[b]->mFinal;
	    }
	  //	  else	    m=mData->getTransform();
	      //	      m=m.transposed();
	      AGVector3 n((m*AGVector4(mData->normal[*i],0)).dim3());
	      AGVector3 p((m*AGVector4(mData->pos[*i],1)).dim3());
				      
	      glNormal3fv(n);
	      glTexCoord2fv(mData->uv[*i]);
	      glVertex3fv(p);
	      /*	      
	    }
	  else
	    {
	      glNormal3fv(mData->normal[*i]);
	      glTexCoord2fv(mData->uv[*i]);
	      glVertex3fv(mData->pos[*i]);
	      }*/
	}
      
      glEnd();
      glPopMatrix();


    }
#endif
  glPopMatrix();
}

void AnimMesh::advance(float time)
{
  mTime+=25*time;

  while(mTime>mData->animTime)
    mTime-=mData->animTime;
}


AnimMeshData *AnimMesh::getData()
{
  return mData;
}


void AnimMesh::setTransform(const AGMatrix4 &m)
{
  mTransform=m;
  assert(mData);
  mComplete=mData->getTransform()*mTransform;
}

void AnimMesh::setPos(const AGVector3 &p)
{
  mPos=AGVector4(p,1);
  mTransform=AGMatrix4(mRot[3],mRot.dim3())*AGMatrix4(mPos);

  assert(mData);
  mComplete=mData->getTransform()*mTransform;
}
void AnimMesh::setRotation(const AGVector3 &r,float a)
{
  mRot=AGVector4(r,a);

  mTransform=AGMatrix4(a,r)*AGMatrix4(mPos);

  assert(mData);
  mComplete=mData->getTransform()*mTransform;
}

void AnimMesh::setRotation(float r)
{
  setRotation(AGVector3(0,0,1),r+180);
}



void AnimMesh_markfunc(void *ptr)
{
  if(!ptr)
    return;

  AnimMeshData *d=static_cast<AnimMesh*>(ptr)->getData();
  if(d->mRubyObject)
    rb_gc_mark(d->mRUBY);
}


