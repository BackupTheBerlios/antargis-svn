#include "anim_mesh.h"
#include "scene.h"
#include "ag_texturecache.h"

#include "ag_debug.h"

#include <math.h>

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
{/*
  std::cout<<angles[0]<<"  "<<angles[1]<<"  "<<angles[2]<<std::endl;
  std::cout<<"cos(-1.5708)="<<cos(-1.5708)<<std::endl;
 */

  double cr = cos( angles[0] );
  double sr = sin( angles[0] );
  double cp = cos( angles[1] );
  double sp = sin( angles[1] );
  double cy = cos( angles[2] );
  double sy = sin( angles[2] );
  /*
  std::cout<<"cr:"<<cr<<std::endl;
  std::cout<<"sr:"<<sr<<std::endl;
  std::cout<<"cp:"<<cp<<std::endl;
  std::cout<<"sp:"<<sp<<std::endl;
  std::cout<<"cy:"<<cy<<std::endl;
  std::cout<<"sy:"<<sy<<std::endl;
  */
  // FIXME: maybe the coord must be switched

  m.set(0,0,( float )( cp*cy ));
  m.set(0,1,( float )( cp*sy ));
  m.set(0,2,( float )( -sp ));
  
  double srsp = sr*sp;
  double crsp = cr*sp;

  //  std::cout<<"srsp:"<<srsp<<std::endl;
  //  std::cout<<"crsp:"<<crsp<<std::endl;
  
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
  /*
  
        float vec[3];

        vec[0] = pVect[0]*m_matrix[0]+pVect[1]*m_matrix[1]+pVect[2]*m_matrix[2];
        vec[1] = pVect[0]*m_matrix[4]+pVect[1]*m_matrix[5]+pVect[2]*m_matrix[6];
        vec[2] = pVect[0]*m_matrix[8]+pVect[1]*m_matrix[9]+pVect[2]*m_matrix[10];

        memcpy( pVect, vec, sizeof( float )*3 );*/
}

void inverseTranslate(AGVector3 &v, const AGMatrix4 &m )
{
  /*        pVect[0] = pVect[0]-m_matrix[12];
        pVect[1] = pVect[1]-m_matrix[13];
        pVect[2] = pVect[2]-m_matrix[14];*/
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

	  /*
	  AGMatrix4 m;
	  setRotation(m,AGVector3(rx,ry,rz));
	  setTranslation(m,AGVector3(x,y,z)*scale);

	  frame->trans.push_back(m);*/
	  frames.push_back(frame);
	}
    }

  setupJoints();
}

void AnimMeshData::apply(const AGMatrix4 &m)
{
  return;
  for(std::vector<AGVector3>::iterator i=pos.begin();i!=pos.end();i++)
    (*i)=(m*AGVector4(*i,1)).dim3();
  for(std::vector<AGVector3>::iterator i=normal.begin();i!=normal.end();i++)
    (*i)=(m*AGVector4(*i,0)).dim3();
}


void AnimMeshData::setupJoints()
{
  //  return;
  //  int i;
  for (std::vector<Bone*>::iterator i = bones.begin(); i!=bones.end(); i++ )
    {
      Bone& joint = **i;
      
      /*      cdebug("ID:"<<joint.id);
      cdebug("rot:"<<joint.rot.toString());
      cdebug("pos:"<<joint.pos.toString());
      */
      cdebug("relative:"<<joint.mRelative.toString());
      
      //      cdebug("REL:");
      setRotation(joint.mRelative, joint.rot);
      //      cdebug(joint.mRelative.toString());
      setTranslation(joint.mRelative, joint.pos);
      //      cdebug(joint.mRelative.toString());

      if(joint.parent)
	joint.mAbsolute=joint.parent->mAbsolute*joint.mRelative;
      //joint.mAbsolute=joint.mRelative*joint.parent->mAbsolute;
      else
	joint.mAbsolute=joint.mRelative;

      //      cdebug(joint.id);
      //      cdebug(joint.parent);
      //cdebug(joint.mAbsolute.toString());


    }

  for (size_t i=0;i<pos.size();i++)
    {
      if(bone[i]!=-1)
	{
	  AGMatrix4 m(bones[bone[i]]->mAbsolute);
	  AGVector3 test=pos[i];
	  inverseTranslate(pos[i],m);
	  inverseRotate(pos[i],m);

	  cdebug(test.toString()<<"   "<<(m*AGVector4(pos[i],1)).toString());

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


  if(false)
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
  else
    {
      // calculate 
      std::vector<AGMatrix4> ms;

      while(mTime>mData->animTime)
	mTime-=mData->animTime;

      // first get two key frames
      KeyFrame *f0,*f1;

      cdebug(mTime);
      std::vector<KeyFrame*>::iterator i=mData->frames.begin();
      cdebug((*i)->time);
      while((*i)->time<=mTime && i!=mData->frames.end())
	i++;
      cdebug((*i)->time);
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
	  cdebug("FFINAL:"<<k);
	  //	  cdebug(final.toString());
	  AGMatrix4 final(mData->bones[k]->mRelative);
	  AGMatrix4 trans;
	  AGVector3 rot,pos;

	  cdebug("relativ:"<<final.toString());

	  rot=f0->rot[k]*i2 + f1->rot[k]*interpol;
	  pos=f0->pos[k]*i2 + f1->pos[k]*interpol;

	  //	  cdebug(f0->rot[k].toString()<<"   "<<f1->rot[k].toString()<<"   "<<rot.toString());
	  cdebug(f0->pos[k].toString()<<"   "<<f1->pos[k].toString()<<"   "<<pos.toString());
	  cdebug("pos:"<<pos.toString());
	  cdebug("rot:"<<rot.toString());

	  setRotation(trans,rot);
	  setTranslation(trans,pos);

		   
	  final=final*trans;
	  //	  final=trans*final;
	  cdebug("trans:"<<trans.toString());
	  cdebug("final:"<<final.toString());

	  //	  final*=trans;
	  if(mData->bones[k]->parent)
	    {
	      //  cdebug("parent:"<<mData->bones[k]->parent->id);
	      //	      final=mData->bones[k]->parent->mFinal*final;
	      final=mData->bones[k]->parent->mFinal*final;
	    }

	  mData->bones[k]->mFinal=final;
	  cdebug("rfinal:"<<final.toString());
	  /*
	  cdebug("FINAL:");
	  cdebug(final.toString());
	  cdebug("ABS:");
	  cdebug(mData->bones[k]->mAbsolute.toString());
	  */
	}
      

      // paint with transform

      glPushMatrix();

      //      glScalef(0.1,0.1,0.1);
      glBegin(GL_TRIANGLES);
      
      // for a start do a simple drawing 
      for(std::vector<size_t>::iterator i=mData->indices.begin();i!=mData->indices.end();i++)
	{
	  int b=mData->bone[*i];
	  if(b>=0)
	    {
	      AGMatrix4 m(mData->bones[b]->mFinal);
	      //	      m=m.transposed();
	      AGVector3 n((m*AGVector4(mData->normal[*i],0)).dim3());
	      AGVector3 p((m*AGVector4(mData->pos[*i],1)).dim3());
				      
	      glNormal3fv(n);
	      glTexCoord2fv(mData->uv[*i]);
	      glVertex3fv(p);
	      
	    }
	  else
	    {
	      glNormal3fv(mData->normal[*i]);
	      glTexCoord2fv(mData->uv[*i]);
	      glVertex3fv(mData->pos[*i]);
	    }
	}
      
      glEnd();
      glPopMatrix();


    }
}
/*
void AnimMesh::padvance(KeyFrame *f0,KeyFrame *f1,float i)
{
  for (int i = 0; i < bones.size(); i++ )
	{
	  AGVector3 transVec;
	  AGMatrix4 transform;
	  int frame;

	  Joint *pJoint = bones[i];
		
	  pJoint->m_currentTranslationKeyframe = frame;

		if ( frame == 0 )
			memcpy( transVec, pJoint->m_pTranslationKeyframes[0].m_parameter, sizeof ( float )*3 );
		else if ( frame == pJoint->m_numTranslationKeyframes )
			memcpy( transVec, pJoint->m_pTranslationKeyframes[frame-1].m_parameter, sizeof ( float )*3 );
		else
		{
			assert( frame > 0 && frame < pJoint->m_numTranslationKeyframes );

			const Model::Keyframe& curFrame = pJoint->m_pTranslationKeyframes[frame];
			const Model::Keyframe& prevFrame = pJoint->m_pTranslationKeyframes[frame-1];

			float timeDelta = curFrame.m_time-prevFrame.m_time;
			float interpValue = ( float )(( time-prevFrame.m_time )/timeDelta );

			transVec[0] = prevFrame.m_parameter[0]+( curFrame.m_parameter[0]-prevFrame.m_parameter[0] )*interpValue;
			transVec[1] = prevFrame.m_parameter[1]+( curFrame.m_parameter[1]-prevFrame.m_parameter[1] )*interpValue;
			transVec[2] = prevFrame.m_parameter[2]+( curFrame.m_parameter[2]-prevFrame.m_parameter[2] )*interpValue; 
		}

		frame = pJoint->m_currentRotationKeyframe;
		while ( frame < pJoint->m_numRotationKeyframes && pJoint->m_pRotationKeyframes[frame].m_time < time )
		{
			frame++;
		}
		pJoint->m_currentRotationKeyframe = frame;

		if ( frame == 0 )
			transform.setRotationRadians( pJoint->m_pRotationKeyframes[0].m_parameter );
		else if ( frame == pJoint->m_numRotationKeyframes )
			transform.setRotationRadians( pJoint->m_pRotationKeyframes[frame-1].m_parameter );
		else
		{
			assert( frame > 0 && frame < pJoint->m_numRotationKeyframes );

			const Model::Keyframe& curFrame = pJoint->m_pRotationKeyframes[frame];
			const Model::Keyframe& prevFrame = pJoint->m_pRotationKeyframes[frame-1];
			
			float timeDelta = curFrame.m_time-prevFrame.m_time;
			float interpValue = ( float )(( time-prevFrame.m_time )/timeDelta );

			assert( interpValue >= 0 && interpValue <= 1 );

			float rotVec[3];

			rotVec[0] = prevFrame.m_parameter[0]+( curFrame.m_parameter[0]-prevFrame.m_parameter[0] )*interpValue;
			rotVec[1] = prevFrame.m_parameter[1]+( curFrame.m_parameter[1]-prevFrame.m_parameter[1] )*interpValue;
			rotVec[2] = prevFrame.m_parameter[2]+( curFrame.m_parameter[2]-prevFrame.m_parameter[2] )*interpValue;

			transform.setRotationRadians( rotVec );
		}

		transform.setTranslation( transVec );
		Matrix relativeFinal( pJoint->m_relative );
		relativeFinal.postMultiply( transform );

		if ( pJoint->m_parent == -1 )
			pJoint->m_final.set( relativeFinal.getMatrix());
		else
		{
			pJoint->m_final.set( m_pJoints[pJoint->m_parent].m_final.getMatrix());
			pJoint->m_final.postMultiply( relativeFinal );
		}
	}


}
*/
void AnimMesh::advance(float time)
{
  //  mTime=0;
  //  return;
  mTime+=time;

  while(mTime>mData->animTime)
    mTime-=mData->animTime;
}


AnimMeshData *AnimMesh::getData()
{
  return mData;
}

























void AnimMesh_markfunc(void *ptr)
{
  if(!ptr)
    return;

  AnimMeshData *d=static_cast<AnimMesh*>(ptr)->getData();
  if(d->mRubyObject)
    rb_gc_mark(d->mRUBY);
}



/* DOCUMENTATION:











*/






