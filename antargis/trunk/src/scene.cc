#include "renderer.h"
#include "scene.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <ag_debug.h>
#include <algorithm>

#include <math.h>

std::set<Scene*> gScenes;

std::set<Scene*> getScenes()
{
  return gScenes;
}

bool PickNode::operator<(const PickNode &n) const
{
  return camDist<n.camDist;
}

class SortDistance
{
  AGVector3 cam;
public:
  SortDistance(AGVector3 c):cam(c){}

  bool operator()(const SceneNode *n1,const SceneNode *n2)
  {
    AGVector3 m1=const_cast<SceneNode*>(n1)->bbox().base+const_cast<SceneNode*>(n1)->bbox().dir*0.5;
    AGVector3 m2=const_cast<SceneNode*>(n2)->bbox().base+const_cast<SceneNode*>(n2)->bbox().dir*0.5;

    return (m1-cam).length2()<(m2-cam).length2();
  }
};

class SortOrder
{
public:
  SortOrder(){}

  bool operator()(const SceneNode *n1,const SceneNode *n2)
  {
    return n1->getOrder()<n2->getOrder();
  }
};


Scene::Scene(int w,int h)
{
  windowWidth=w;
  windowHeight=h;
  white=AGVector4(1,1,1,1);
  black=AGVector4(0,0,0,1);
  inited=false;
  orthoShadow=true;
  
  cameraPosition=AGVector4(0,-20,20);
  lightPosition=AGVector4( -20, -13, 31,1);
  lightPosition=AGVector4( -50, -50, 100,1);
  scenePosition=AGVector4(0,0,0,1);

  cdebug("SHADOW:"<<(int)GLEE_ARB_shadow);
  cdebug("SHADOW_AMB:"<<(int)GLEE_ARB_shadow_ambient);
  
  GLeeInit();
  
  if(getRenderer()->canShadow())
    mShadow=1;
  else
    mShadow=0;
  
  mRubyObject=false;
  gScenes.insert(this);
  init();

  calcCameraView();
}

Scene::~Scene()
{
  // tell nodes, that I'm no longer there :-)
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    (*i)->setScene(0); 
  gScenes.erase(this);
}

size_t Scene::getTriangles() const
{
  return mTriangles;
}


void Scene::draw()
{
  getRenderer()->setCurrentScene(this);
  assertGL;
 
  mTriangles=0;
  calcCameraView();

  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    (*i)->sort(scenePosition);

  
  if(mShadow)
    {
      calcShadowMap();
      initScene();
      drawShadow();
    }
  else
    {
      initScene();
      drawScene();
    }

  getRenderer()->setCurrentScene(0);
}

void Scene::setShadow(int v)
{
  if(getRenderer()->canShadow())
    {
      mShadow=v;
      cdebug(mShadow);
    }
}
int Scene::getShadow() const
{
  return mShadow;
}


void Scene::addNode(SceneNode *node)
{
  if(mNodeSet.find(node)==mNodeSet.end())
    {
      mNodes.push_back(node);
      mNodeSet.insert(node);
      node->setScene(this);
    }
}

void Scene::removeNode(SceneNode *node)
{
  if(mNodeSet.find(node)!=mNodeSet.end())
    {
      Nodes::iterator i=std::find(mNodes.begin(),mNodes.end(),node);
      mNodes.erase(i);
      mNodeSet.erase(node);
      node->setScene(0);
    }
}

void Scene::clear()
{
  for(std::vector<SceneNode*>::iterator i=mNodes.begin();i!=mNodes.end();i++)
    (*i)->setScene(0);
  TRACE;
  mNodes.clear();
  mNodeSet.clear();
}

  // (mx,my,0)
void Scene::setCamera(AGVector4 v)
{
  scenePosition=v;
  scenePosition[3]=1;
  calcCameraView();
}

void Scene::advance(float time)
{
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    (*i)->advance(time);
}


void Scene::calcCameraView()
{
  glLoadIdentity();
  gluLookAt(cameraPosition[0]+scenePosition[0],cameraPosition[1]+scenePosition[1],cameraPosition[2]+scenePosition[2],
	    scenePosition[0],scenePosition[1],scenePosition[2],
	    0,0,1);
  glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);
  

  if(mShadow==2)
    {
      // PSM
      // calculation of lightposition is somehow crappy


      // PSMs
      //  lightPosition=AGVector4( -2.0, -3, 5.1,1)*100;
      
      // light View Matrix
      glLoadIdentity();
      
      AGVector4 lp=lightPosition;
      //    lp[
      lp[3]=1;
      lp=cameraProjectionMatrix*cameraViewMatrix*lp;
      
      lp/=lp[3];
      
      // it is something like (12,-10,10)
      
      
      
      
      lp=AGVector4(-0.5,1.5,-0.5,1); // should be something like this 
      lp*=100;
      
      
      //lp=AGVector4(-2,2,-2,1);
      gluLookAt(lp[0], lp[1], lp[2],
		0,0,0,
		0.0f, 1.0f, 0.0f);
      glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
      
      lightViewMatrix=lightViewMatrix*cameraProjectionMatrix*cameraViewMatrix;
      // light projection Matrix
      glLoadIdentity();
      //    glOrtho(-10,10,-15,20,10,1000);
      cdebug(lp.toString());
      float s2=sqrt(2.0f);
      float ldist=lp.length3();
      
      glOrtho(-s2,s2,-s2,s2,ldist-2*s2,ldist+10);//1,10);//ldist-2*s2,ldist+10*s2);
      
      
      //very old:glOrtho(-1,2,-1.5,3,700,750);
      //      glOrtho(-1,2,-1,1,2,8);
      
      glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);
    }
  else
    {
      //  lightPosition=AGVector4( -1.0, -3, 5.1,1);
      
      // calc light view,too
      // light View Matrix
      glLoadIdentity();
      gluLookAt(lightPosition[0]+scenePosition[0], lightPosition[1]+scenePosition[1], lightPosition[2]+scenePosition[2],
		scenePosition[0],scenePosition[1],scenePosition[2],
		0.0f, 1.0f, 0.0f);
      glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
      
      
      // light projection Matrix
      glLoadIdentity();
      
      if(orthoShadow)
	// 1=   2=   3=front 4=back
	glOrtho(-15,20,-20,46, 70,200); // left,rigt, bottom,top - these are estimated values // for 1024
      //	glOrtho(-1.3,1.3,-2,5, 2,10); // left,rigt, bottom,top - these are estimated values // for 1024
      else
	gluPerspective(45.0f, 1.0f, 2.0f, 8.0f);
      
      glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);
    }
}


void Scene::calcShadowMap()
{
  assertGL;
  AGMatrix4 frustum=getFrustum();

  getRenderer()->beginShadowComputation();
  
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    {
      if((*i)->bbox().collides(frustum) && (*i)->visible())
	{
	  (*i)->drawDepth();
	  mTriangles+=(*i)->getTriangles();
	}
    }
  getRenderer()->endShadowComputation();
}

void Scene::initScene()
{
  glDisable(GL_COLOR_MATERIAL);
  glClear(GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(cameraProjectionMatrix);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //  glLoadMatrixf(cameraViewMatrix);
  
  glViewport(0, 0, windowWidth, windowHeight);
  
  //Use dim light to represent shadowed areas
  //  cdebug( lightPosition.toString()<<"   "<<scenePosition.toString()<<"  "<< (lightPosition+scenePosition).toString());

  AGVector4 l=lightPosition+scenePosition;
  l[3]=1;

  glLightfv(GL_LIGHT1, GL_POSITION, l);
  glLightfv(GL_LIGHT1, GL_AMBIENT, AGVector4(0.1,0.1,0.1,1));//white*0.05f);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, AGVector4(0.3,0.3,0.3,1));//white*0.3f);//*0.2f);
  glLightfv(GL_LIGHT1, GL_SPECULAR, black);
  glEnable(GL_LIGHT1);

  AGVector4 lightPosition2=AGVector4( 0, 0, -50,1);

  //  glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2+scenePosition);
  glLightfv(GL_LIGHT2, GL_AMBIENT, black);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, AGVector4(0.7,0.7,0.7,1));//white*0.7f);//*0.2f);
  glLightfv(GL_LIGHT2, GL_SPECULAR, white);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, AGVector4(0,0,-1,0));
  glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,0);
  glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,0);
  glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION,0);
  glEnable(GL_LIGHT2);



  glEnable(GL_LIGHTING);

  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT3);
  glDisable(GL_LIGHT4);
  glDisable(GL_LIGHT5);

  glLoadMatrixf(cameraViewMatrix);
  
  glBindTexture(GL_TEXTURE_2D,0);

  //  calcCameraView();
  
}

AGMatrix4 Scene::getFrustum()
{
  return cameraProjectionMatrix*cameraViewMatrix;
}


void Scene::drawScene()
{
  AGMatrix4 frustum=getFrustum();
  
  //2nd pass - Draw from camera's point of view

  //  glAlphaFunc(GL_GREATER,0.9f);
  //  glEnable(GL_ALPHA_TEST);
  // draw scene with texturing and so

  int drawn=0;

  Nodes sorted=mNodes;

#ifndef OLD_SORTING
  //sort(sorted.begin(),sorted.end(),SortDistance(cameraPosition.dim3()));
  sort(sorted.begin(),sorted.end(),SortOrder());

  for(Nodes::iterator i=sorted.begin();i!=sorted.end();i++)
    {
      if(!(*i)->transparent())
	{
	  if((*i)->visible() && (*i)->bbox().collides(frustum))
	    {
	      (*i)->draw();
	      mTriangles+=(*i)->getTriangles();
	      drawn++;
	    }
	}
    }
  
  //  glDisable(GL_ALPHA_TEST);
  //  glAlphaFunc(GL_ALWAYS,1);

  sort(sorted.begin(),sorted.end(),SortDistance(cameraPosition.dim3()));

  for(Nodes::reverse_iterator i=sorted.rbegin();i!=sorted.rend();i++)
    {
      if((*i)->transparent())
	{
	  if((*i)->visible() && (*i)->bbox().collides(frustum))
	    {
	      (*i)->draw();
	      mTriangles+=(*i)->getTriangles();
	      drawn++;
	    }
	}
    }
  //  if(mNodes.size())
    //    cdebug("drawn:"<<(float(drawn)/mNodes.size()*100)<<"%");
  


#else
  sort(sorted.begin(),sorted.end(),SortOrder());

  for(Nodes::iterator i=sorted.begin();i!=sorted.end();i++)
    {
      if((*i)->visible() && (*i)->bbox().collides(frustum))
	{
	  (*i)->draw();
	  mTriangles+=(*i)->getTriangles();
	  drawn++;
	}
      
    }


#endif
  
}
void Scene::drawShadow()
{
  assertGL;

  getRenderer()->beginShadowDrawing();
  
  drawScene();

  getRenderer()->endShadowDrawing();

  assertGL;
}

void Scene::init()
{
  inited=true;
  
  // camera projection matrix
  glLoadIdentity();
  gluPerspective(45.0f, ((float)windowWidth)/windowHeight, 10.0f, 50.0f);
  glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
  //  calcCameraView();
}


void Scene::mapChanged()
{
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    (*i)->mapChanged();
}

AGVector3 Scene::getCameraDirTo(const AGVector3 &p) const
{
  return (cameraPosition+scenePosition).dim3()-p;
}


void Scene::pickDraw()
{
  glDisable(GL_CULL_FACE);
  GLuint name=1;
  pickNames.clear();
  AGMatrix4 frustum=cameraPickMatrix*cameraViewMatrix;
  

  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    {
      if((*i)->visible() && (*i)->bbox().collides(frustum))
	{
	  glPushName(name);
	  (*i)->drawPick();
	  glPopName();
	  pickNames.insert(std::make_pair(name,*i));
	  name++;
	}
    }

  glEnable(GL_CULL_FACE);
  
}

Scene::PickResult Scene::pick(float x,float y,float w,float h)
{
  size_t bufsize=4000;
  GLuint buffer[bufsize-1];
  
  glSelectBuffer(bufsize,buffer);
  glRenderMode(GL_SELECT);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  assertGL;
  gluPickMatrix(x,windowHeight-y,h,w,getViewport());

  assertGL;
  
  glMultMatrixf(cameraProjectionMatrix);
  glGetFloatv(GL_PROJECTION_MATRIX, cameraPickMatrix);
  assertGL;
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMultMatrixf(cameraViewMatrix);
  glInitNames();
  assertGL;

  pickDraw();
  assertGL;
  
  
  // back to normality
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glFlush();
  assertGL;
  
  int hits=glRenderMode(GL_RENDER);
  assertGL;
  PickResult r=processHits(hits,buffer,x+w/2,windowHeight-(y+h/2));
  std::sort(r.begin(),r.end());
  return r;
}

Scene::PickResult Scene::processHits (int hits, GLuint *buffer,float x,float y)
{
  PickResult result;
  if(hits==0)
    return result;

   unsigned int i, j;
   GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

   ptr = (GLuint *) buffer;
   minZ = 0xffffffff;
   for (i = 0; i < (unsigned int)hits; i++) { 
      names = *ptr;
      ptr++;
      if (*ptr < minZ) {
	numberOfNames = names;
	minZ = *ptr;
	//	cdebug("minz:"<<minZ);
	ptrNames = ptr+2;

	GLuint *mptr=ptrNames;
	for(j=0;j<numberOfNames;j++,mptr++)
	  {
	    PickNode n;
	    n.node=pickNames[*mptr];
	    n.camDist=minZ/float(0xFFFFFFFF); // (0-1)

	    // get world-position
	    GLdouble x,y,z;

	    GLdouble modelview[16],projection[16];
	    for(size_t i=0;i<16;i++)
	      {
		modelview[i]=((float*)cameraViewMatrix)[i];
		projection[i]=((float*)cameraPickMatrix)[i];
	      }

	    gluUnProject(x,y,n.camDist,modelview,projection,getViewport(),&x,&y,&z);
	    n.pos=AGVector4(x,y,z,1);
	    n.camDist=(n.pos-cameraPosition).length3();

	    result.push_back(n);
	  }

      }
      
      ptr += names+2;
   }

   return result;
}


Viewport Scene::getViewport() const
{
  Viewport p;
  p.viewport[0]=0;
  p.viewport[1]=0;
  p.viewport[2]=windowWidth;
  p.viewport[3]=windowHeight;
  return p;
}


float Scene::width() const
{
  return windowWidth;
}
float Scene::height() const
{
  return windowHeight;
}

void Scene::mark()
{
  Scene::Nodes::iterator i=mNodes.begin();

  for(;i!=mNodes.end();i++)
    {
      markObject(*i);
    }
}

AGMatrix4 Scene::getInvCameraView() const
{
  AGMatrix4 n,m;

  int x,y;
  for(y=0;y<3;y++)
    {
      for(x=0;x<3;x++)
	n.set(x,y,cameraViewMatrix.get(y,x));
      m.set(3,y,-cameraViewMatrix.get(3,y));
    }

  n=n*m;

  return n;
}

AGMatrix4 Scene::getLightComplete() const
{
  float bias[]={0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f};        //bias from [-1, 1] to [0, 1]
  static AGMatrix4 biasMatrix(bias);
  return biasMatrix*lightProjectionMatrix*lightViewMatrix;
}

AGMatrix4 Scene::getLightView() const
{
  return lightViewMatrix;
}
AGMatrix4 Scene::getLightProj() const
{
  return lightProjectionMatrix;
}

AGVector4 Scene::getCamera() const
{
  return scenePosition;
}

AGPoint Scene::getPosition(const AGVector4 &v) const
{
  GLdouble x,y,z;
  
  GLdouble modelview[16],projection[16];
  for(size_t i=0;i<16;i++)
    {
      modelview[i]=((const float*)cameraViewMatrix)[i];
      projection[i]=((const float*)cameraProjectionMatrix)[i];
    }
  

  gluProject(v[0],v[1],v[2],modelview,projection,getViewport(),&x,&y,&z);
  return AGPoint(x,windowHeight-y);
}


void addToAllScenes(SceneNode *n)
{
  for(Scenes::iterator i=gScenes.begin();i!=gScenes.end();i++)
    (*i)->addNode(n);
}
void removeFromAllScenes(SceneNode *n)
{
  for(Scenes::iterator i=gScenes.begin();i!=gScenes.end();i++)
    (*i)->removeNode(n);
}
