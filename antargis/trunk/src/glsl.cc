#include "glsl.h"


AntVertexProgram::AntVertexProgram(const std::string &pFile)
{
  if(ok())
    {
      vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
      std::string s=loadFile(pFile);
      
      const char*a=s.c_str();
      const GLcharARB**x=&a;
      glShaderSourceARB(vertexShader, 1, x,NULL);
      glCompileShaderARB(vertexShader);
      
    }
}

bool AntVertexProgram::ok() const
{
  GLeeInit();
  //    gleeInit();
  return (GLEE_ARB_vertex_shader && GLEE_ARB_fragment_shader);
}


AntFragProgram::AntFragProgram(const std::string &pFile)
{
  if(ok())
    {
      fragShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
      std::string s=loadFile(pFile);
      const char*a=s.c_str();
      const GLcharARB**x=&a;
      glShaderSourceARB(fragShader, 1, x,NULL);
      glCompileShaderARB(fragShader);
      
    }
}

bool AntFragProgram::ok() const
{
  GLeeInit();
  return (GLEE_ARB_vertex_shader && GLEE_ARB_fragment_shader);
}


AntShaderProgram::AntShaderProgram(const std::string &pVertexFile,const std::string &pFragFile):
  vertex(pVertexFile),frag(pFragFile)
{
  p = glCreateProgramObjectARB();
  glAttachObjectARB(p,vertex.vertexShader);
  glAttachObjectARB(p,frag.fragShader);
  
  glLinkProgramARB(p);
}

void AntShaderProgram::enable()
{
  glUseProgramObjectARB(p);
}
void AntShaderProgram::disable()
{
  glUseProgramObjectARB(0);
}


AntWaterShader::AntWaterShader():
  AntShaderProgram("data/shaders/minimal.vert","data/shaders/minimal.frag")
{
  enable();
  loc=glGetUniformLocationARB(p,"time");
  //      assertGL;
  disable();
}
void AntWaterShader::update(float time)
{
  enable();
  t+=time;
  glUniform1fARB(loc, t);
  //      assertGL;
  disable();
}



