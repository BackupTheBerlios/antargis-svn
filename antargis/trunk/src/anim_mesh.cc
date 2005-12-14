#include "anim_mesh.h"
#include "scene.h"
#include "ag_texturecache.h"

#include "ag_debug.h"
#include "ag_xml.h"
#include "ag_fs.h"
#include "ant_config.h"
#include "entity.h"

#include <math.h>

bool useAnimation()
{
  return true;
  return getConfig()->get("animationType")!="none";
}

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


AGVector3 Bone::interpolate(const std::vector<KeyFrame> &frames,float t)
{
  size_t i;
  for(i=0;i<frames.size();i++)
    {
      if(frames.size()>i+1)
	{
	  if(frames[i+1].time>t)
	    break;
	}
      else
	break;
    }
  size_t j=i+1;
  if(j>=frames.size())
    return frames[i].v;
  
  float t0=frames[i].time;
  float t1=frames[j].time;

  float i0=(t-t0)/(t1-t0);
  float i1=1-i0;

  return frames[i].v*i1 + frames[j].v*i0;
}
AGVector3 Bone::interpolateRot(float t)
{
  return interpolate(rFrames,t);
}
AGVector3 Bone::interpolateTrans(float t)
{
  return interpolate(tFrames,t);
}




AnimMeshData::AnimMeshData(const std::string &xmlfile):
  animShader("data/shaders/anim.vert","data/shaders/anim.frag"),mArray(&animShader)
{
  Document doc(xmlfile);

  Node &root=doc.root();

  events=false;

  animate=true;
  if(root.get("debug")=="true")
    animate=false;
  if(!useAnimation())
    animate=false;

  cdebug("loading from:"<<root.get("model"));

  loadAnt3(loadFile(root.get("model")),toFloat(root.get("scale")),root.get("texture"));
  
  // load animations
  
  cdebug("animTime:"<<animTime);

  Node::NodeVector anims=root.get_children("animation");
  for(Node::NodeVector::iterator i=anims.begin();i!=anims.end();i++)
    {
      float begin=toFloat((*i)->get("begin"))-1;
      float end=toFloat((*i)->get("end"))-1;
      float fps=toFloat((*i)->get("fps"));
      assert(fps>0);
      assert(begin>=0);
      assert(begin<animTime);
      assert(begin<=end);
      assert(end<animTime);
      mAnimations[(*i)->get("name")]=Animation(begin,end,fps);

      mAnimations[(*i)->get("name")].loop=((*i)->get("loop")!="false");

      // parse events
      Node::NodeVector enodes=(*i)->get_children("event");
      for(Node::NodeVector::iterator j=enodes.begin();j!=enodes.end();j++)
	{
	  frameEvents.insert(std::make_pair(toInt((*j)->get("frame")),(*j)->get("name")));
	}
      
    }

  if(anims.size()==0)
    {
      std::cerr<<"There are no animations in "<<xmlfile<<std::endl;
      throw std::string("no animations found in xmlfile");
    }
  mName=xmlfile;
}

AnimMeshData::~AnimMeshData()
{
  CTRACE;
  cdebug(mName);
}

AGBox3 AnimMeshData::bbox() const
{
  return mBBox;
}

void AnimMeshData::setEvents(bool e)
{
  events=e;
}


void AnimMeshData::loadAnt3(const std::string &instr,float scale,const std::string &tex)
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

      AGVector3 v(x,y,z);
      v*=scale;
      mBBox.include(v);
      pos.push_back(v);
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

  Uint32 nbones,frames;
  l>>frames>>nbones;

  for(size_t k=0;k<bone.size();k++)
    {
      if(bone[k]==-1)
	bonef.push_back(nbones);
      else
	bonef.push_back(bone[k]);
    }


  animTime=frames;

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

      // read rot-frames
      Sint32 frames;
      l>>frames;
      KeyFrame frame;
      for(Sint32 r=0;r<frames;r++)
	{
	  float x,y,z,t;
	  l>>t>>x>>y>>z;
	  frame.time=t;
	  frame.v=AGVector3(x,y,z);
	  bone->rFrames.push_back(frame);
	}

      l>>frames;
      for(Sint32 r=0;r<frames;r++)
	{
	  float x,y,z,t;
	  l>>t>>x>>y>>z;
	  frame.time=t;
	  frame.v=AGVector3(x,y,z)*scale;
	  bone->tFrames.push_back(frame);
	}



      bones.push_back(bone);
    }

  if(animate)
    setupJoints();

  setupArray();
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


void AnimMeshData::setupArray()
{
  for(size_t i=0;i<pos.size();i++)
    mArray.addVertex(AGVector4(pos[i],1),AGVector4(1,1,1,1),normal[i],uv[i]);
  for(size_t i=0;i<indices.size();i+=3)
    mArray.addTriangle(indices[i],indices[i+1],indices[i+2]);

  // set boneIDs
  mArray.addAttribute("bones",bonef);
}


/////////////////////////////////////////////////////////////////////////
// AnimMesh
/////////////////////////////////////////////////////////////////////////


AnimMesh::AnimMesh(AnimMeshData *data):mData(data),mMatrices(data->bones.size()+1),mShaderMatrices(20)
{
  curKey=0;
  mTime=0;
  
  assert(mData->mAnimations.size()>0);
  mAnimName=mData->mAnimations.begin()->first;
  mAnimation=&mData->mAnimations.begin()->second;

  entity=0;

}

AnimMesh::~AnimMesh()
{
  //  CTRACE;
}

void AnimMesh::setEntity(AntEntity *e)
{
  entity=e;
}


void AnimMesh::drawDepth()
{
   return;
  drawPrivate(false);
}

void AnimMesh::draw()
{
  //  return;
  drawPrivate(true);
}
void AnimMesh::drawPick()
{
  drawPrivate(false);
}

// at first try a simple animation without shaders
void AnimMesh::drawPrivate(bool textured)
{
  bool fast=true;
  if(textured)
    {
      glColor4f(1,1,1,1);
      glBindTexture(GL_TEXTURE_2D,mData->mTexture.getTextureID());
      glEnable(GL_LIGHTING);
    }
  else
    fast=false; // FIXME: add shader for non-textured rendering

  glPushMatrix();

  glTranslatef(mPos[0],mPos[1],mPos[2]);
  //  if(!fast)
    glRotatef(mRot[3],mRot[0],mRot[1],mRot[2]);


  if(fast)
    {
      glMultMatrixf(mData->getTransform());
      if(textured)
	{
	  glDisable(GL_COLOR_MATERIAL);
	  mData->animShader.enable();
	  //mData->animShader.sendAttribute("bones",mData->bonef);
	  mData->animShader.sendUniform("matrices",mShaderMatrices);
	  
	  mData->mArray.draw();

	  mData->animShader.disable();
	  glEnable(GL_COLOR_MATERIAL);

	}
      else
	mData->mArray.drawPick();
    }

  else if(!mData->animate)
    {
      glBegin(GL_TRIANGLES);
      
      // for a start do a simple drawing 
      for(std::vector<size_t>::iterator i=mData->indices.begin();i!=mData->indices.end();i++)
	{
	  AGMatrix4 m(mData->getTransform());
	  AGVector3 p((m*AGVector4(mData->pos[*i],1)).dim3());
	  if(textured)
	    {
	      AGVector3 n((m*AGVector4(mData->normal[*i],0)).dim3());
	      
	      glNormal3fv(n);
	      glTexCoord2fv(mData->uv[*i]);
	    }
	  glVertex3fv(p);
	  
	}
      
      glEnd();
    }
  else
    {

      // paint with transform

      glMultMatrixf(mData->getTransform());

      glBegin(GL_TRIANGLES);
      
      // for a start do a simple drawing 
      for(std::vector<size_t>::iterator i=mData->indices.begin();i!=mData->indices.end();i++)
	{
	  int b=mData->bone[*i];
	  AGMatrix4 m;
	  if(b>=0)
	    {
	      m=mMatrices[b];
	    }
	  AGVector3 p((m*AGVector4(mData->pos[*i],1)).dim3());
	  if(textured)
	    {
	      AGVector3 n((m*AGVector4(mData->normal[*i],0)).dim3());
	      
	      glNormal3fv(n);
	      glTexCoord2fv(mData->uv[*i]);
	    }
	  glVertex3fv(p);
	}
      
      glEnd();


    }
  
  glPopMatrix();
}

void AnimMesh::advance(float time)
{
  if(!mData->animate)
    return;
  float oldTime=mTime;

  mTime+=mAnimation->fps*time;

  if(mAnimation->len>0)
    {
      if(mAnimation->loop)
	while(mTime>mAnimation->end)
	  {
	    mTime-=mAnimation->len;
	    //	    if(entity)
	    //	      entity->animationEvent("looped");
	  }
      else
	mTime=std::min(mTime,mAnimation->end);
    }
  else
    mTime=mAnimation->begin;

  // check events
  if(entity && mData->frameEvents.size())
    {
      //      cdebug("ARG");
      //      if(mData->frameEvents.size())
      //	cdebug(oldTime<<"  "<<mTime);
      if(oldTime>mTime)
	{
	  for(std::map<int,std::string>::iterator i=mData->frameEvents.begin();i!=mData->frameEvents.end();i++)
	    if(i->first>=oldTime || i->first<mTime)
	      entity->animationEvent(i->second);
	}
      else
	{
	  for(std::map<int,std::string>::iterator i=mData->frameEvents.begin();i!=mData->frameEvents.end();i++)
	    {
	      //	      cdebug("i:"<<i->first);
	      if(i->first>=oldTime && i->first<mTime)
		{
		  //		  cdebug("EVENT:"<<i->second);
		  entity->animationEvent(i->second);
		}
	    }
	}

    }


  update();
}

void AnimMesh::update()
{
  // this doesn't take much of rendering-time
  // calculate 
  
  while(mTime>mData->animTime)
    mTime-=mData->animTime;
  
  for(size_t k=0;k<mData->bones.size();k++)
    {
      Bone *bone=mData->bones[k];
      AGMatrix4 final(bone->mRelative);
      AGMatrix4 trans;
      AGVector3 rot,pos;

      // first get surrounding keyframes (rot & trans) for this bone 
      // and calculate ri0,ri1 and ti0,ti1 (1-ti0 == ti1)

      rot=bone->interpolateRot(mTime);
      pos=bone->interpolateTrans(mTime);

      ::setRotation(trans,rot);
      setTranslation(trans,pos);
      
      
      final=final*trans;
      
      if(mData->bones[k]->parent)
	final=mMatrices[mData->bones[k]->parent->id]*final;
      
      mMatrices[k]=final;
    }
  mMatrices[mData->bones.size()]=AGMatrix4();

  for(size_t k=0;k<mData->bones.size();k++)
    mShaderMatrices[k]=mData->getTransform()*mMatrices[k];
  mShaderMatrices[mData->bones.size()]=mData->getTransform()*mMatrices[mData->bones.size()];


  for(size_t k=0;k<mData->bones.size();k++)
    {
      mShaderMatrices[k]=mMatrices[k];
      //      cdebug(k<<std::endl<<mShaderMatrices[k].toString());
    }
  //  throw int();
  mShaderMatrices[mData->bones.size()]=mMatrices[mData->bones.size()];



  /*  for(size_t k=0;k<=mData->bones.size();k++)
    mShaderMatrices[k]=AGMatrix4();
  for(size_t k=0;k<=mData->bones.size();k++)
  mShaderMatrices[k]=mData->getTransform();*/
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

std::string AnimMesh::getAnimation() const
{
  return mAnimName;
}

bool AnimMesh::setAnimation(const std::string &pName)
{
  if(mAnimName==pName)
    return true;
  if(mData->mAnimations.find(pName)==mData->mAnimations.end())
    {
      cdebug(std::string("Animation ")+pName+" is known here!");
      return false;
    }

  mAnimation=&(mData->mAnimations[pName]);
  mAnimName=pName;
  mTime=mAnimation->begin;
  return true;
}

AGBox3 AnimMesh::bbox()
{
  return mData->bbox()+mPos.dim3();
}


void AnimMesh::mark()
{
  markObject(mData);
}


/* fast rendering


1) bone-association
prepare buffers

like vertexbuffer use

p= program
loc = glGetAttribLocationARB(p,"height"); // get saving handle
glEnableVertexAttribArrayARB(loc); // add array
glVertexAttribPointerARB(loc,1,GL_FLOAT,0,0,heights); // set attributes (for each vertex an attribute)


2) give matrices
with 
void glUniformMatrix4fvARB(GLint location,
                           GLsizei count,
                           GLboolean transpose,
                           GLfloat *value)
count==bone-count
*/

