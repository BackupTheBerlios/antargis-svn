#include "glsl.h"
#include "ag_debug.h"
#include "scene.h"

int GLSL_ok=-1;
bool ok()
{
//  return false;
  // do not check in each call, because this is slow!!!
  if(GLSL_ok<0)
    {
      GLeeInit();
      GLSL_ok=(GLEE_ARB_vertex_shader && GLEE_ARB_fragment_shader && GLEE_ARB_shading_language_100);
    }
  
  return GLSL_ok;
}

void printInfoLog(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

        glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                         &infologLength);

    if (infologLength > 0)
    {
      printf("GLSL ERROR:\n");
        infoLog = (char *)malloc(infologLength);
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
                printf("%s\n",infoLog);
        free(infoLog);
    }
}


AntVertexProgram::AntVertexProgram(const std::string &pFile)
{
  //  CTRACE;
  if(ok())
    {
      vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
      std::string s=loadFile(pFile);
      
      const char*a=s.c_str();
      const GLcharARB**x=&a;
      glShaderSourceARB(vertexShader, 1, x,NULL);
      glCompileShaderARB(vertexShader);
      printInfoLog(vertexShader);
      
    }
}

AntVertexProgram::~AntVertexProgram()
{
  CTRACE;
  if(ok() && !hasQuit())
    glDeleteObjectARB(vertexShader);
}

AntFragProgram::AntFragProgram(const std::string &pFile)
{
  //  CTRACE;
  if(ok())
    {
      fragShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
      std::string s=loadFile(pFile);
      const char*a=s.c_str();
      const GLcharARB**x=&a;
      glShaderSourceARB(fragShader, 1, x,NULL);
      glCompileShaderARB(fragShader);
      printInfoLog(fragShader);
      
    }
}

AntFragProgram::~AntFragProgram()
{
  CTRACE;
  if(ok() && !hasQuit())
    glDeleteObjectARB(fragShader);
}

AntShaderProgram::AntShaderProgram(const std::string &pVertexFile,const std::string &pFragFile):
  vertex(pVertexFile),frag(pFragFile)
{
  //  CTRACE;
  if(ok())
    {
      p = glCreateProgramObjectARB();
      glAttachObjectARB(p,vertex.vertexShader);
      glAttachObjectARB(p,frag.fragShader);
      
      glLinkProgramARB(p);
      printInfoLog(p);

      assert(p);
    }
  on=false;
  matrixBuf=new float[16*100];
  name=pVertexFile+":"+pFragFile;
}

AntShaderProgram::~AntShaderProgram()
{
  disable();
  CTRACE;
  cdebug("name:"<<name);
  if(ok() && !hasQuit())
    glDeleteObjectARB(p);
  cdebug("name:"<<name);
  delete [] matrixBuf;
  cdebug("name:"<<name);
}


void AntShaderProgram::enable()
{
  if(ok())
    {
      glUseProgramObjectARB(p);
      on=true;
    }
}
void AntShaderProgram::disable()
{
  if(ok())
    {
      glUseProgramObjectARB(0);
      on=false;
    }
}

void AntShaderProgram::update(float time)
{
  if(ok())
    {
      enable();
      doUpdate(time);
      disable();
    }
}

void AntShaderProgram::doUpdate(float time)
{
}

void AntShaderProgram::sendUniform(const std::string &pName,int i)
{
  glUniform1iARB(getLoc(pName),i);
}
void AntShaderProgram::sendUniform(const std::string &pName,float f)
{
  glUniform1fARB(getLoc(pName),f);
}
void AntShaderProgram::sendUniform(const std::string &pName,const AGVector3 &m)
{
  glUniform3fARB(getLoc(pName),m[0],m[1],m[2]);
}
void AntShaderProgram::sendUniform(const std::string &pName,const AGVector4 &m)
{
  glUniform4fARB(getLoc(pName),m[0],m[1],m[2],m[3]);
}
void AntShaderProgram::sendUniform(const std::string &pName,const AGMatrix4 &m)
{
  glUniformMatrix4fvARB(getLoc(pName),1,false,m);
}
void AntShaderProgram::sendUniform(const std::string &pName,const std::vector<AGMatrix4> &m)
{
  assert(m.size()<100);
  float *p=matrixBuf;

  for(size_t i=0;i<m.size();i++)
    {
      const float *s=m[i];
      for(size_t j=0;j<16;j++)
	*(p++)=*(s++);
    }
  assertGL;
  glUniformMatrix4fvARB(getLoc(pName),m.size(),false,matrixBuf);
  assertGL;
}

GLint AntShaderProgram::getLoc(const std::string &pName)
{
  // register only once !
  
  std::map<std::string,GLint>::iterator i=locations.find(pName);
  if(i!=locations.end())
    return i->second;
  
  GLint k=glGetUniformLocationARB(p,pName.c_str());
  locations.insert(std::make_pair(pName,k));
  return k;
}

GLint AntShaderProgram::getAttr(const std::string &pName)
{
  //  TRACE;
  // register only once !
  assert(on);
  std::map<std::string,GLint>::iterator i=attrs.find(pName);
  if(i!=attrs.end())
    return i->second;
  
  assertGL;
  GLint k=glGetAttribLocationARB(p,pName.c_str());
  //  cdebug("k:"<<k);
  if(k<0)
    {
      cdebug("ERROR:attribute could be get! Maybe it was not defined in the vertex-shader? :"<<pName);
    }
  assertGL;
  attrs.insert(std::make_pair(pName,k));
  return k;
}

void AntShaderProgram::sendAttribute(const std::string &pName,const std::vector<float> &vf)
{
  assertGL;
  GLint loc=getAttr(pName);
  assertGL;
  glEnableClientState(GL_VERTEX_ARRAY);
  assertGL;
  glEnableVertexAttribArrayARB(loc); // add array
  assertGL;

  glVertexAttribPointerARB(loc,1,GL_FLOAT,0,0,&vf[0]); // set attributes (for each vertex an attribute)
  assertGL;
}



///////////////////////////////////////////////////////////////////////////
// AntShadowShader
//
///////////////////////////////////////////////////////////////////////////

void AntShadowShader::doUpdate(float time)
{
  if(ok())
    {
      Renderer *r=getRenderer();
      sendUniform("shadowTex",r->getShadowUnit());
      sendUniform("normalTex",r->getNormalUnit());
      sendUniform("lightComplete",r->getCurrentScene()->getLightComplete());
    }
}


void AntShadowShader::enable()
{
  AntShaderProgram::enable();
  if(ok())
    {
      Scene *scene=getRenderer()->getCurrentScene();

      glActiveTexture(getRenderer()->getShadowUnit()); // shadow unit

      glMatrixMode(GL_TEXTURE);
      glPushMatrix();

      // FIXME: move bias to frag-shader, as this shall reduce z-fighting
      float bias[]={0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f};        //bias from [-1, 1] to [0, 1]
      glLoadMatrixf(bias);

      glMultMatrixf(scene->getLightProj());
      glMultMatrixf(scene->getLightView());

      glActiveTexture(getRenderer()->getNormalUnit()); // tex unit

      glMatrixMode(GL_MODELVIEW);
    }
}
void AntShadowShader::disable()
{
  AntShaderProgram::disable();
  if(ok())
    {
      glActiveTexture(getRenderer()->getShadowUnit()); // shadow unit
      glMatrixMode(GL_TEXTURE);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glActiveTexture(getRenderer()->getNormalUnit()); // tex unit
    }
}


///////////////////////////////////////////////////////////////////////////
// AntWaterShader
//
///////////////////////////////////////////////////////////////////////////


AntWaterShader::AntWaterShader():
  AntShaderProgram("data/shaders/simplewater.vert","data/shaders/simplewater.frag")
{
}
void AntWaterShader::doUpdate(float time)
{
  if(ok())
    {
      AntShaderProgram::doUpdate(time);
      
      t+=time;
      sendUniform("time", t);
    }
}



