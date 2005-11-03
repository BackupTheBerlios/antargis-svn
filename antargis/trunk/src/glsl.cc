#include "glsl.h"
#include "ag_debug.h"


int GLSL_ok=-1;
bool ok()
{
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
        infoLog = (char *)malloc(infologLength);
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
                printf("%s\n",infoLog);
        free(infoLog);
    }
}


AntVertexProgram::AntVertexProgram(const std::string &pFile)
{
  CTRACE;
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
  if(ok())
    glDeleteObjectARB(vertexShader);
}

AntFragProgram::AntFragProgram(const std::string &pFile)
{
  CTRACE;
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
  if(ok())
    glDeleteObjectARB(fragShader);
}

AntShaderProgram::AntShaderProgram(const std::string &pVertexFile,const std::string &pFragFile):
  vertex(pVertexFile),frag(pFragFile)
{
  CTRACE;
  if(ok())
    {
      p = glCreateProgramObjectARB();
      glAttachObjectARB(p,vertex.vertexShader);
      glAttachObjectARB(p,frag.fragShader);
      
      glLinkProgramARB(p);
      printInfoLog(p);
    }
}

AntShaderProgram::~AntShaderProgram()
{
  CTRACE;
  if(ok())
    glDeleteObjectARB(p);
}


void AntShaderProgram::enable()
{
  if(ok())
    glUseProgramObjectARB(p);
}
void AntShaderProgram::disable()
{
  if(ok())
    glUseProgramObjectARB(0);
}

AntWaterShader::AntWaterShader():
  AntShaderProgram("data/shaders/textured.vert","data/shaders/textured.frag")
			      //  AntShaderProgram("data/shaders/minimal.vert","data/shaders/minimal.frag")
{
  if(ok())
    {
      enable();
      loc=glGetUniformLocationARB(p,"time");
      //      assertGL;
      disable();
    }
}
void AntWaterShader::update(float time)
{
  if(ok())
    {
      enable();
      t+=time;
      glUniform1fARB(loc, t);
      //      assertGL;
      disable();
    }
}



