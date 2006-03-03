#include "anim_mesh.h"
#include "scene.h"
#include "ag_texturecache.h"
#include "ag_rendercontext.h"

#include "ag_debug.h"
#include "ag_xml.h"
#include "ag_fs.h"
#include "ant_config.h"
#include "entity.h"

#include <math.h>

bool useShaderAnimation(); // imported from anim_mesh_data.cc


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



/////////////////////////////////////////////////////////////////////////
// AnimMesh
/////////////////////////////////////////////////////////////////////////


AnimMesh::AnimMesh(Scene *pScene,AnimMeshData *data):
  SceneNode(pScene,AGVector4(),data->bbox()),
  mData(data),mMatrices(data->bones.size()+1),mShaderMatrices(20)
{
  curKey=0;
  mTime=0;
  
  assert(mData->mAnimations.size()>0);
  mAnimName=mData->mAnimations.begin()->first;
  mAnimation=&mData->mAnimations.begin()->second;

  entity=0;
  setOrder(MESH_Z);

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
  //     return;
  drawPrivate(false,false);
}

void AnimMesh::draw()
{
  //  return;
  drawPrivate(true,false);
}
void AnimMesh::drawPick()
{
  drawPrivate(false,true);
}

// at first try a simple animation without shaders
void AnimMesh::drawPrivate(bool textured, bool mem)
{
  AGRenderContext c;
//  bool fast=true;
  if(textured)
    {
      c.setTexture(mData->mTexture.glTexture());
      /*
      glColor4f(1,1,1,1);
      glBindTexture(GL_TEXTURE_2D,mData->mTexture.getTextureID());
      glEnable(GL_LIGHTING);*/
    }
  //  else
  //    fast=false; // FIXME: add shader for non-textured rendering
  c.setLighting(true);
  c.begin();
  glPushMatrix();

  AGVector4 p=getPos();
  glTranslatef(p[0],p[1],p[2]);//mPos[0],mPos[1],mPos[2]);
  //  if(!fast)
  glRotatef(mRot[3],mRot[0],mRot[1],mRot[2]);


  if(useShaderAnimation())
    {
      glMultMatrixf(mData->getTransform());
      if(textured)
	{
	  //	  glDisable(GL_COLOR_MATERIAL);
	  mData->animShader.enable();
	  //mData->animShader.sendAttribute("bones",mData->bonef);
	  mData->animShader.sendUniform("matrices",mShaderMatrices);
	  
	  mData->mArray.draw();

	  mData->animShader.disable();
	  //	  glEnable(GL_COLOR_MATERIAL);

	}
      else if(mem)
	mData->mArray.drawPick();
      else
	{
	  mData->animShaderDepth.enable();
	  mData->animShaderDepth.sendUniform("matrices",mShaderMatrices);
	  mData->mArrayDepth.setColors(false);
	  mData->mArrayDepth.draw();

	  mData->animShaderDepth.disable();
	}
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
  SceneNode::setPos(p);

  //  getScene()->prepareUpdate(this);

  //  mPos=AGVector4(p,1);
  mTransform=AGMatrix4(mRot[3],mRot.dim3())*AGMatrix4(getPos());//p);//mPos);

  assert(mData);
  mComplete=mData->getTransform()*mTransform;

  //  getScene()->updatePos(this);
}
void AnimMesh::setRotation(const AGVector3 &r,float a)
{
  mRot=AGVector4(r,a);

  mTransform=AGMatrix4(a,r)*AGMatrix4(getPos());

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

/*AGBox3 AnimMesh::bbox() const
{
  return mData->bbox()+mPos.dim3();
  }*/


void AnimMesh::mark()
{
  markObject(mData);
}

size_t AnimMesh::getTriangles() const
{
  return mData->indices.size()/3;
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

