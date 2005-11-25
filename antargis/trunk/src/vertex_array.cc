#include "vertex_array.h"
#include <SDL_opengl.h>
#include <assert.h>
#include <ag_debug.h>

VertexArray::VertexArray():bbox(AGVector3(),AGVector3())
{
  bColor=true;
  mBuffers=false;//true;
  mChanged=false;

  mVertexBuffer=0;
  mColorBuffer=0;
  mNormalBuffer=0;
  mTexBuffer=0;
  mIndexBuffer=0;
}

VertexArray::~VertexArray()
{
  //  CTRACE;
  if(mBuffers && mVertexBuffer!=0) //mChanged==false)
    {
      glDeleteBuffersARB( 1, &mVertexBuffer );
      glDeleteBuffersARB( 1, &mColorBuffer );
      glDeleteBuffersARB( 1, &mNormalBuffer );
      glDeleteBuffersARB( 1, &mTexBuffer );
      glDeleteBuffersARB( 1, &mIndexBuffer );
    }
}


void VertexArray::addVertex(AGVector4 pVertex, AGVector4 pColor, AGVector3 pNormal, AGVector2 pTex)
{
  mVertices.push_back(pVertex);
  mColors.push_back(pColor);
  mNormals.push_back(pNormal);
  mTexCoords.push_back(pTex);
  mChanged=true;
  if(mVertices.size()==1)
    bbox=AGBox3(pVertex.dim3(),AGVector3(0,0,0));
  else
    bbox.include(pVertex.dim3());
}

void VertexArray::addTriangle(size_t p0,size_t p1,size_t p2)
{
  assert(p0<mVertices.size());
  assert(p1<mVertices.size());
  assert(p2<mVertices.size());
  mIndices.push_back(p0);
  mIndices.push_back(p1);
  mIndices.push_back(p2);
  mChanged=true;
}

void VertexArray::init()
{
  if(mBuffers)
    {
      glGenBuffersARB( 1, &mVertexBuffer );
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, mVertexBuffer );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, mVertices.size()*sizeof(AGVector4), &(mVertices[0]), GL_STATIC_DRAW_ARB );
      
      glGenBuffersARB( 1, &mColorBuffer );
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, mColorBuffer );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, mColors.size()*sizeof(AGVector4), &(mColors[0]), GL_STATIC_DRAW_ARB );
      
      glGenBuffersARB( 1, &mNormalBuffer );
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, mNormalBuffer );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, mNormals.size()*sizeof(AGVector3), &(mNormals[0]), GL_STATIC_DRAW_ARB );
      
      glGenBuffersARB( 1, &mTexBuffer );
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, mTexBuffer );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, mTexCoords.size()*sizeof(AGVector2), &(mTexCoords[0]), GL_STATIC_DRAW_ARB );
      
      glGenBuffersARB( 1, &mIndexBuffer );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mIndexBuffer );
      glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mIndices.size()*sizeof(Uint16), &(mIndices[0]), GL_STATIC_DRAW_ARB );

      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);


    }
  mChanged=false;
}

void VertexArray::setColors(bool color)
{
  bColor=color;
}

void VertexArray::draw()
{
  if(mChanged)
    init();

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  if(bColor)
    glEnableClientState(GL_COLOR_ARRAY);
  else
    glDisableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);


  if(mBuffers)
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, mNormalBuffer );
      glNormalPointer(GL_FLOAT, 0, 0);

      glClientActiveTexture(GL_TEXTURE1);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, mTexBuffer);
      glTexCoordPointer(2, GL_FLOAT, 0, 0);

      glClientActiveTexture(GL_TEXTURE0);

      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, mTexBuffer);
      glTexCoordPointer(2, GL_FLOAT, 0, 0);

      glBindBufferARB( GL_ARRAY_BUFFER_ARB, mVertexBuffer);
      glVertexPointer(4, GL_FLOAT, 0, 0);
      if(bColor)
	{
	  glBindBufferARB( GL_ARRAY_BUFFER_ARB, mColorBuffer);
	  glColorPointer(4, GL_FLOAT, 0, 0);
	}
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mIndexBuffer);
      glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_SHORT,0);

    }
  else
    {
      //      cdebug("too much work");
      glNormalPointer(GL_FLOAT, 0, &(mNormals[0]));
      glTexCoordPointer(2, GL_FLOAT, 0, &(mTexCoords[0]));
      glVertexPointer(4, GL_FLOAT, 0, &(mVertices[0]));
      if(bColor)
	glColorPointer(4, GL_FLOAT, 0, &(mColors[0]));
      glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_SHORT,
		     &(mIndices[0]));
  }

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

void VertexArray::drawPick()
{
  if(true)
    {
      glBegin(GL_TRIANGLES);
      for(std::vector<Uint16>::iterator i=mIndices.begin();i!=mIndices.end();i++)
	glVertex4fv(mVertices[*i]);
      glEnd();
    }
  else
    {
      // FIXME: some this doesn't - what the heck!
      if(mChanged)
	init();
      
      
      glEnableClientState(GL_VERTEX_ARRAY);
      
      
      // disable textures
      glClientActiveTexture(GL_TEXTURE1);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glClientActiveTexture(GL_TEXTURE0);
      
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      
      {
	cdebug(mVertices.size());
	cdebug(mIndices.size());
	glBindBufferARB( GL_ARRAY_BUFFER_ARB,0);
	glNormalPointer(GL_FLOAT,0,0);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB,0);
	glTexCoordPointer(2,GL_FLOAT,0,0);
	glVertexPointer(4, GL_FLOAT, 0, &(mVertices[0]));
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_SHORT,
		       &(mIndices[0]));
      }
      
      glDisableClientState(GL_VERTEX_ARRAY);
    }
}



AGVector4 VertexArray::lineHit(const AGLine3 &pLine) const
{
  AGVector4 mfalse(0,0,0,0);
  //  cdebug("box:"<<bbox.toString());
  if(!bbox.collides(pLine))
    return mfalse;

  for(size_t i=0;i<mIndices.size();i+=3)
    {
      AGVector4 v0=mVertices[mIndices[i]];
      AGVector4 v1=mVertices[mIndices[i+1]];
      AGVector4 v2=mVertices[mIndices[i+2]];

      AGTriangle3 t(v0.dim3(),v1.dim3(),v2.dim3());
      AGVector4 r;
      r=t.collide(pLine);
      if(r[3]==1)
	{
	  return r;
	}
    }
  return mfalse;
}

void VertexArray::update()
{
  if(mBuffers && mVertexBuffer!=0) //mChanged==false)
    {
      glDeleteBuffersARB( 1, &mVertexBuffer );
      glDeleteBuffersARB( 1, &mColorBuffer );
      glDeleteBuffersARB( 1, &mNormalBuffer );
      glDeleteBuffersARB( 1, &mTexBuffer );
      glDeleteBuffersARB( 1, &mIndexBuffer );
      mVertexBuffer=0;
    }
  mChanged=true;
}
void VertexArray::clear()
{
  /*  if(mBuffers && mVertexBuffer!=0) //mChanged==false)
    {
      glDeleteBuffersARB( 1, &mVertexBuffer );
      glDeleteBuffersARB( 1, &mColorBuffer );
      glDeleteBuffersARB( 1, &mNormalBuffer );
      glDeleteBuffersARB( 1, &mTexBuffer );
      glDeleteBuffersARB( 1, &mIndexBuffer );
      }*/
  mVertices.clear();
  mColors.clear();
  mNormals.clear();
  mTexCoords.clear();
  mIndices.clear();
  update();
}

void VertexArray::setBuffers(bool pBuffers)
{
  mBuffers=false;
}


size_t VertexArray::getIndex(size_t i)
{
  return mIndices[i];
}
AGVector4 VertexArray::getVertex(size_t i)
{
  return mVertices[i];
}
AGVector4 VertexArray::getColor(size_t i)
{
  return mColors[i];
}
AGVector3 VertexArray::getNormal(size_t i)
{
  return mNormals[i];
}
  AGVector2 VertexArray::getTexCoord(size_t i)
{
  return mTexCoords[i];
}

void VertexArray::setTexCoord(size_t i,const AGVector2 &t)
{
  mTexCoords[i]=t;
  update();
}




VertexArrayShader::VertexArrayShader(AntShaderProgram *_p):p(_p)
{
  aInited=false;
}
void VertexArrayShader::addAttribute(const std::string &pName,const std::vector<float> &a)
{
  as[pName]=new std::vector<float>(a);
  aInited=false;
}

void VertexArrayShader::draw()
{
  p->enable();
  if(!aInited)
    aInit();

  attach();
  VertexArray::draw();
  p->disable();
}

void VertexArrayShader::attach()
{
  for(std::map<std::string,unsigned int>::iterator i=aids.begin();i!=aids.end();i++)
    {
      GLint loc=p->getAttr(i->first);
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableVertexAttribArrayARB(loc); // add array
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, i->second);
      glVertexAttribPointerARB(loc,1,GL_FLOAT,0,0,0);
      //      glTexCoordPointer(2, GL_FLOAT, 0, 0);
    }
}

void VertexArrayShader::aInit()
{
  for(std::map<std::string,std::vector<float>*>::iterator i=as.begin();i!=as.end();i++)
    {
      unsigned int id;

      glGenBuffersARB( 1, &id);
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, id);
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, i->second->size()*sizeof(float), &((*i->second)[0]), GL_STATIC_DRAW_ARB );

      aids[i->first]=id;
    }
  aInited=true;
}
