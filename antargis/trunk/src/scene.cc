#include "scene.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <ag_debug.h>
#include <algorithm>

#include <math.h>

const GLenum shadowTexUnit= GL_TEXTURE2;
const GLenum baseTexUnit= GL_TEXTURE0;

bool PickNode::operator<(const PickNode &n) const
{
  return camDist<n.camDist;
}


Scene::Scene(int w,int h)
{
  windowWidth=w;
  windowHeight=h;
  shadowMapSize=512;
  white=AGVector4(1,1,1,1);
  black=AGVector4(0,0,0,0);
  inited=false;
  orthoShadow=true;
  
  cameraPosition=AGVector4(0,-20,20);
  lightPosition=AGVector4( -20, -13, 31,1);
  scenePosition=AGVector4(0,0,0,1);
  mShadow=1;
  mRubyObject=false;
}

void Scene::draw()
{
  if(!inited)
    init();
 
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
  //  cdebug("Triangles:"<<mTriangles);
}

void Scene::setShadow(int v)
{
  mShadow=v;
  cdebug(mShadow);
}
int Scene::getShadow() const
{
  cdebug(mShadow);
  return mShadow;
}


void Scene::addNode(SceneNode *node)
{
  if(mNodeSet.find(node)==mNodeSet.end())
    {
      mNodes.push_back(node);
      mNodeSet.insert(node);
    }
}
void Scene::removeNode(SceneNode *node)
{
  if(mNodeSet.find(node)!=mNodeSet.end())
    {
      Nodes::iterator i=std::find(mNodes.begin(),mNodes.end(),node);
      mNodes.erase(i);
      mNodeSet.erase(node);
    }
}

void Scene::clear()
{
  TRACE;
  mNodes.clear();
  mNodeSet.clear();
}

  // (mx,my,0)
void Scene::setCamera(AGVector4 v)
{
  scenePosition=v;
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
	glOrtho(-13,20,-20,46, 20,120); // left,rigt, bottom,top - these are estimated values // for 1024
      //	glOrtho(-1.3,1.3,-2,5, 2,10); // left,rigt, bottom,top - these are estimated values // for 1024
      else
	gluPerspective(45.0f, 1.0f, 2.0f, 8.0f);
      
      glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);
    }
}


void Scene::calcShadowMap()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(lightProjectionMatrix);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(lightViewMatrix);
  
  //Use viewport the same size as the shadow map
  glViewport(0, 0, shadowMapSize, shadowMapSize);
  
  //Draw back faces into the shadow map
  glCullFace(GL_FRONT);
  
  //Disable color writes, and use flat shading for speed
  glShadeModel(GL_FLAT);
  glColorMask(0, 0, 0, 0);
  
  
  //Draw the scene
  // Offset the drawing a little back, so that slopy surfaces don't get shadowed
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1,1);
  
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    {
      (*i)->drawDepth();
      mTriangles+=(*i)->getTriangles();
    }
  
  glDisable(GL_POLYGON_OFFSET_FILL);
  
  //Read the depth buffer into the shadow map texture
  glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
  glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);
  
  //restore states
  assertGL;
  glCullFace(GL_BACK);
  
  glShadeModel(GL_SMOOTH);
  glColorMask(1, 1, 1, 1);
}

void Scene::initScene()
{
  glDisable(GL_COLOR_MATERIAL);
  glClear(GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(cameraProjectionMatrix);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(cameraViewMatrix);
  
  glViewport(0, 0, windowWidth, windowHeight);
  
  //Use dim light to represent shadowed areas
  glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT1, GL_AMBIENT, white*0.2f);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, white*0.2f);
  glLightfv(GL_LIGHT1, GL_SPECULAR, black);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);
  
  glBindTexture(GL_TEXTURE_2D,0);
}

void Scene::drawScene()
{
  //2nd pass - Draw from camera's point of view

  // draw scene with texturing and so
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    {
      (*i)->draw();
      mTriangles+=(*i)->getTriangles();
    }

  
}
void Scene::drawShadow()
{
  
  glActiveTexture(shadowTexUnit);

  //  glMatrixMode(GL_TEXTURE);
  //  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  // draw a flat shadow over 
  glDisable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  
  //Calculate texture matrix for projection
  //This matrix takes us from eye space to the light's clip space
  //It is postmultiplied by the inverse of the current view matrix when specifying texgen
  float bias[]={0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f};        //bias from [-1, 1] to [0, 1]
  static AGMatrix4 biasMatrix(bias);
  AGMatrix4 textureMatrix=biasMatrix*lightProjectionMatrix*lightViewMatrix;
  
  //Set up texture coordinate generation.
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.getRow(0));
  glEnable(GL_TEXTURE_GEN_S);
  
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.getRow(1));
  glEnable(GL_TEXTURE_GEN_T);
  
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.getRow(2));
  glEnable(GL_TEXTURE_GEN_R);
  
  glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.getRow(3));
  glEnable(GL_TEXTURE_GEN_Q);
  
  //Bind & enable shadow map texture
  glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
  glEnable(GL_TEXTURE_2D);
  
  //Enable shadow comparison


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
  
  //Shadow comparison should be true (ie not in shadow) if r<=texture
  //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);//GREATER);//GL_GEQUAL);
  
  //Shadow comparison should generate an INTENSITY result
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);//INTENSITY);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, 1.0f-0.3f);//shadowAlpha);

  //Set alpha test to discard false comparisons
  //  glAlphaFunc(GL_GREATER,0.99f);//LEQUAL, 0.999f);

  
  glColor4f(0,0,0,0.3);

  glActiveTexture(baseTexUnit);


  // draw scene with texturing and so
  //  glTranslatef(0.004,0,0);
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    {
      (*i)->draw();
      mTriangles+=(*i)->getTriangles();
    }

  glDisable(GL_POLYGON_OFFSET_FILL);

  glActiveTexture(shadowTexUnit);
  glBindTexture(GL_TEXTURE_2D,0);

  glDisable(GL_ALPHA_TEST);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_GEN_Q);
  
  glActiveTexture(baseTexUnit);


}

void Scene::init()
  {
    inited=true;

    // camera projection matrix
    glLoadIdentity();
    gluPerspective(45.0f, ((float)windowWidth)/windowHeight, 10.0f, 50.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
    
    // init texture

    glGenTextures(1, &shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexImage2D(   GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadowMapSize, shadowMapSize, 0,
		    GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  }


Scene::PickResult Scene::lineHit(const AGLine3 &pLine)
{
  PickResult f;
  AGVector4 r;
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    {
      r=(*i)->lineHit(pLine);
      if(r[3]==1)
	{
	  float d=(r-(cameraPosition+scenePosition)).length3();
	  PickNode n={r,*i,d};
	  
	  f.push_back(n);
	}
    }

  std::sort(f.begin(),f.end());
    
  return f;
}

AGLine3 Scene::getLine(int x,int y)
{
  float sx,sy;

  sx=float(x)/windowWidth;
  sy=1.0f-float(y)/windowHeight;
  sx=x;
  sy=windowHeight-y;
  GLdouble px,py,pz;
  GLdouble px2,py2,pz2;

  double model[16],proj[16];
  GLint view[4];
  view[0]=view[1]=0;
  view[2]=windowWidth;
  view[3]=windowHeight;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glLoadIdentity();
  gluLookAt(cameraPosition[0]+scenePosition[0],cameraPosition[1]+scenePosition[1],cameraPosition[2]+scenePosition[2],
	    scenePosition[0],scenePosition[1],scenePosition[2],
	    0,0,1);
  glGetDoublev(GL_MODELVIEW_MATRIX, model);

  glLoadIdentity();
  gluPerspective(45.0f, ((float)windowWidth)/windowHeight, 1.0f, 100.0f);
  glGetDoublev(GL_MODELVIEW_MATRIX, proj);

  glPopMatrix();

  gluUnProject(sx,sy,0.1,
	       model,proj,view,
	       &px,&py,&pz);

  gluUnProject(sx,sy,0.99,
	       model,proj,view,
	       &px2,&py2,&pz2);

  return AGLine3(AGVector3(px,py,pz),AGVector3(px2,py2,pz2));
}

void Scene::mapChanged()
{
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    (*i)->mapChanged();
}



void Scene_markfunc(void *ptr)
{
  if(!ptr)
    return;

  SceneNode *cppAnimal;
  VALUE   rubyAnimal;
  Scene *zoo;


  zoo = static_cast<Scene*>(ptr);

  Scene::Nodes::iterator i=zoo->mNodes.begin();

  for(;i!=zoo->mNodes.end();i++)
    {
      cppAnimal = *i;
      if(cppAnimal->mRubyObject)
        {
          rubyAnimal = cppAnimal->mRUBY;
          rb_gc_mark(rubyAnimal);
        }
    }
}
