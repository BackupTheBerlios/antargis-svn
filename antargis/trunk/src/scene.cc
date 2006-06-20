#include "ant_renderer.h"
#include "scene.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <ag_debug.h>
#include <ag_rendercontext.h>
#include <algorithm>

#include <math.h>

#include "quadtree.h"
#include "ag_profiler.h"
#include "ag_main.h"

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


Scene::Scene(int w,int h):
  mTree(new QuadTree<SceneNode>(AGRect2(AGVector2(),AGVector2(w,h)))),
  mCamera(w,h)
{
  white=AGVector4(1,1,1,1);
  black=AGVector4(0,0,0,1);
  
  cdebug("SHADOW:"<<(int)GLEE_ARB_shadow);
  cdebug("SHADOW_AMB:"<<(int)GLEE_ARB_shadow_ambient);
  
  GLeeInit();
  
  if(getRenderer()->canShadow())
    mShadow=1;
  else
    mShadow=0;
  
  mRubyObject=false;
  mEnabled=true;

}

Scene::~Scene()
{
  // tell nodes, that I'm no longer there :-)
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    (*i)->resetScene(); 

  delete mTree;
}

size_t Scene::getDrawnMeshes() const
{
  return mMeshes;
}

size_t Scene::getTriangles() const
{
  return mTriangles;
}

size_t Scene::getPickTriangles() const
{
  return mPickTriangles;
}


void Scene::draw()
{
  if(!mEnabled)
    return;
  AGRenderContext c;
  c.begin(); // reset gl-state

  getRenderer()->setCurrentScene(this);
  assertGL;

  mMeshes=0;
  mTriangles=0;
  mPickTriangles=0;

  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    (*i)->sort(AGVector4(mCamera.getPosition(),1));
  
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
      assert(node->getScene()==this);
      mTree->insert(node);
    }
}

void Scene::updatePos(SceneNode *node)
{
  if(mNodeSet.find(node)==mNodeSet.end())
    throw std::string("Dont know about this!");
  mTree->insert(node);
}

void Scene::prepareUpdate(SceneNode *node)
{
  if(mNodeSet.find(node)==mNodeSet.end())
    throw std::string("Dont know about this!");
  mTree->remove(node);
}


void Scene::removeNode(SceneNode *node)
{
  if(mNodeSet.find(node)!=mNodeSet.end())
    {
      Nodes::iterator i=std::find(mNodes.begin(),mNodes.end(),node);
      mNodes.erase(i);
      mNodeSet.erase(node);
      assert(node->getScene()==this);
      node->resetScene();
      assert(mTree->remove(node));
    }
  else
    {
      throw std::runtime_error("Trying to remove unknown node");
    }
}

void Scene::clear()
{
  for(std::vector<SceneNode*>::iterator i=mNodes.begin();i!=mNodes.end();i++)
    {
      assert((*i)->getScene()==this);
      (*i)->resetScene();
    }
  TRACE;
  mNodes.clear();
  mNodeSet.clear();
  mTree->clear();
}

  // (mx,my,0)
void Scene::setCamera(AGVector4 v)
{
  mCamera.setPosition(v.dim3());
}

void Scene::advance(float time)
{
  STACKTRACE; 

  if(!mEnabled)
    return;
  // advance only in view

  NodeList l=getCurrentNodes();

  for(NodeList::iterator i=l.begin();i!=l.end();i++)
    {
      if((*i)->visible())
	(*i)->advance(time);
    }
}

Scene::NodeList Scene::getCurrentNodes()
{
  AGVector2 p=mCamera.getPosition().dim2();
  NodeList l=mTree->get(AGRect2(p+AGVector2(-30,-30),p+AGVector2(30,30)));

  for(NodeList::iterator i=l.begin();i!=l.end();i++)
    {
      if(mNodeSet.find(*i)==mNodeSet.end())
	cdebug("ERROR:"<<*i);
      assert(mNodeSet.find(*i)!=mNodeSet.end());
    }
  return l;
}


void Scene::calcShadowMap()
{
  STACKTRACE;
  assertGL;
  AGMatrix4 frustum=getFrustum();

  getRenderer()->beginShadowComputation();
  
  NodeList l=getCurrentNodes();

  Nodes sorted;
  std::copy(l.begin(),l.end(),std::back_inserter(sorted));

  sort(sorted.begin(),sorted.end(),SortOrder());

  {
    STACKTRACE;
    NodeList l=getCurrentNodes();
    Nodes sorted;
    std::copy(l.begin(),l.end(),std::back_inserter(sorted));
    
    sort(sorted.begin(),sorted.end(),SortOrder());
    
    for(Nodes::iterator i=sorted.begin();i!=sorted.end();i++)
      {
	{
	  if((*i)->visible())
	    {
	      (*i)->drawDepth();
	      mTriangles+=(*i)->getTriangles();
	    }
	}
    }
  }

  getRenderer()->endShadowComputation();
  assertGL;
}

void Scene::initScene()
{
  glClear(GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(mCamera.getProjection());
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float xfactor=float(getMain()->realWidth())/getMain()->width();
  float yfactor=float(getMain()->realHeight())/getMain()->height();
  
  glViewport(0, 0, mCamera.getWidth()*xfactor, mCamera.getHeight()*yfactor);
  
  //Use dim light to represent shadowed areas

  AGVector4 l=mCamera.getLightPosition();
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

  //  glLoadMatrixf(cameraViewMatrix);
  glLoadMatrixf(mCamera.getModelview());
}

AGMatrix4 Scene::getFrustum()
{
  return mCamera.getProjection()*mCamera.getModelview();
  //return cameraProjectionMatrix*cameraViewMatrix;
}

static GLuint displayList=0;
static bool dlInited=false;

void Scene::drawScene()
{
#ifdef TEST_DL
  if(dlInited)
    {
      glCallList(displayList);
    }
  else
    {
      dlInited=true;
      displayList=glGenLists(1);
      glNewList(displayList,GL_COMPILE);
#endif
  STACKTRACE; 
  AGMatrix4 frustum=getFrustum();
  AntFrustum cFrustum=mCamera.getCameraProjection().getFrustum();
  
  //2nd pass - Draw from camera's point of view

  // draw scene with texturing and so

  int drawn=0;

#ifdef NOQUADTREE
  Nodes sorted=mNodes;
#else

  NodeList l=getCurrentNodes();
  Nodes sorted;
  std::copy(l.begin(),l.end(),std::back_inserter(sorted));

#endif


  {
    STACKTRACE;
    for(Nodes::iterator i=sorted.begin();i!=sorted.end();)
      {
	if(cFrustum.collides((*i)->bbox()))
	  i++;
	else
	  i=sorted.erase(i);
      }
  }

  sort(sorted.begin(),sorted.end(),SortOrder());

  for(Nodes::iterator i=sorted.begin();i!=sorted.end();i++)
    {
      if(!(*i)->transparent())
	{
	  if((*i)->visible())
	    {
	      (*i)->draw();
	      mTriangles+=(*i)->getTriangles();
	      drawn++;
	      mMeshes++;
	    }
	}
    }
  sort(sorted.begin(),sorted.end(),SortDistance(mCamera.getCameraPosition().dim3()));

  for(Nodes::reverse_iterator i=sorted.rbegin();i!=sorted.rend();i++)
    {
      if((*i)->transparent())
	{
	  if((*i)->visible())
	    {
	      (*i)->draw();
	      mTriangles+=(*i)->getTriangles();
	      drawn++;
	      mMeshes++;
	    }
	}
    }
#ifdef TEST_DL
glEndList();
      glCallList(displayList);
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

AGVector3 Scene::getCameraDirTo(const AGVector3 &p) const
{
  return mCamera.getCameraPosition().dim3()-p;
}


void Scene::pickDraw()
{
  STACKTRACE;
  glDisable(GL_CULL_FACE);
  GLuint name=1;
  pickNames.clear();

  AGMatrix4 frustum=cameraPickMatrix*mCamera.getModelview();
  
  NodeList l=getCurrentNodes();

  for(NodeList::iterator i=l.begin();i!=l.end();i++)
    {
      STACKTRACE;
      if((*i)->visible() && (*i)->bbox().collides(frustum))
	{
	  STACKTRACE;
	  glPushName(name);
	  (*i)->drawPick();
	  glPopName();
	  pickNames.insert(std::make_pair(name,*i));
	  name++;
	  mPickTriangles+=(*i)->getTriangles();
	}
    }

  glEnable(GL_CULL_FACE);
  
}

Scene::PickResult Scene::pick(float x,float y,float w,float h)
{
  STACKTRACE;
  size_t bufsize=4000;
  GLuint buffer[bufsize+1];
  
  glSelectBuffer(bufsize,buffer);
  glRenderMode(GL_SELECT);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  assertGL;
  gluPickMatrix(x,mCamera.getHeight()-y,h,w,getViewport());

  assertGL;
  
  glMultMatrixf(mCamera.getProjection());
  glGetFloatv(GL_PROJECTION_MATRIX, cameraPickMatrix);
  assertGL;
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glMultMatrixf(mCamera.getModelview());
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
  PickResult r=processHits(hits,buffer,x+w/2,mCamera.getHeight()-(y+h/2));
  std::sort(r.begin(),r.end());
  return r;
}

Scene::PickResult Scene::processHits (int hits, GLuint *buffer,float px,float py)
{
  STACKTRACE;
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
		modelview[i]=((float*)mCamera.getModelview())[i];
		projection[i]=((float*)cameraPickMatrix)[i];
	      }

	    gluUnProject(px,py,n.camDist,modelview,projection,getViewport(),&x,&y,&z);
	    n.pos=AGVector4(x,y,z,1);
	    n.camDist=(n.pos-mCamera.getCameraPosition()).length3();

	    result.push_back(n);
	  }

      }
      
      ptr += names+2;
   }

   return result;
}


Viewport Scene::getViewport() const
{
  return mCamera.getViewport();
}


float Scene::width() const
{
  return mCamera.getWidth();
}
float Scene::height() const
{
  return mCamera.getHeight();
}

void Scene::mark()
{
  Scene::Nodes::iterator i=mNodes.begin();

  for(;i!=mNodes.end();i++)
    {
      markObject(*i);
    }
}

AGMatrix4 Scene::getLightComplete() const
{
  return mCamera.getLightComplete();
}

AGMatrix4 Scene::getLightView() const
{
  return mCamera.getLightView();
}
AGMatrix4 Scene::getLightProj() const
{
  return mCamera.getLightProjectionMatrix();
}

AGVector4 Scene::getCamera() const
{
  return AGVector4(mCamera.getPosition(),1);
}

AGVector2 Scene::getPosition(const AGVector4 &v) const
{
  GLdouble x,y,z;
  
  GLdouble modelview[16],projection[16];
  for(size_t i=0;i<16;i++)
    {
      modelview[i]=((const float*)mCamera.getModelview())[i];
      projection[i]=((const float*)mCamera.getProjection())[i];
    }

  gluProject(v[0],v[1],v[2],modelview,projection,getViewport(),&x,&y,&z);
  return AGVector2((int)x,((int)mCamera.getHeight()-y));
}



void Scene::setEnabled(bool p)
{
  mEnabled=p;
}

AntCamera &Scene::getCameraObject()
{
  return mCamera;
}
