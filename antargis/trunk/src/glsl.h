#ifndef ANT_GLSL_H
#define ANT_GLSL_H

#include "glee/GLee.h"
#include "ag_tools.h"

class AntVertexProgram
{
 public:
  AntVertexProgram(const std::string &pFile);

 private:
  bool ok() const;

  GLhandleARB vertexShader;

  friend class AntShaderProgram;
};

class AntFragProgram
{
 public:
  AntFragProgram(const std::string &pFile);

 private:
  bool ok() const;

  GLhandleARB fragShader;

  friend class AntShaderProgram;
};


class AntShaderParameter
{
 public:
  
};

class AntShaderProgram
{
  AntVertexProgram vertex;
  AntFragProgram frag;
 protected:  
  GLhandleARB p;

 public:
  AntShaderProgram(const std::string &pVertexFile,const std::string &pFragFile);
  void enable();
  void disable();
};

class AntWaterShader:public AntShaderProgram
{
  float t;
  GLint loc;
 public:
  AntWaterShader();
  void update(float time);
};

#endif
