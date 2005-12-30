#ifndef ANT_GLSL_H
#define ANT_GLSL_H

#include "renderer.h"
#include "ag_tools.h"
#include "ag_triangle.h"

#include <map>

class Scene;

class AntVertexProgram
{
 public:
  AntVertexProgram(const std::string &pFile);
  ~AntVertexProgram();

 private:
  GLhandleARB vertexShader;

  friend class AntShaderProgram;
};

class AntFragProgram
{
 public:
  AntFragProgram(const std::string &pFile);
  AntFragProgram();
  ~AntFragProgram();

  bool valid() const;

 private:
  GLhandleARB fragShader;
  bool mValid;

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
  std::map<std::string,GLint> locations;
  std::map<std::string,GLint> attrs;
  float *matrixBuf;
  bool on;
  std::string name;
 protected:  
  GLhandleARB p;

 public:
  AntShaderProgram(const std::string &pVertexFile,const std::string &pFragFile);
  virtual ~AntShaderProgram();
  virtual void enable();
  virtual void disable();

  void update(float time);
  virtual void doUpdate(float time);

  GLint getLoc(const std::string &pName);
  GLint getAttr(const std::string &pName);

  void sendUniform(const std::string &pName,int i);
  void sendUniform(const std::string &pName,float f);
  void sendUniform(const std::string &pName,const AGVector3 &m);
  void sendUniform(const std::string &pName,const AGVector4 &m);
  void sendUniform(const std::string &pName,const AGMatrix4 &m);
  void sendUniform(const std::string &pName,const std::vector<AGMatrix4> &m);

  void sendAttribute(const std::string &pName,const std::vector<float> &vf);
};

class AntShadowShader:public AntShaderProgram
{
 public:
  AntShadowShader(const std::string &pVertexFile,const std::string &pFragFile);
  void doUpdate(float time);
  virtual void enable();
  virtual void disable();
};

class AntWaterShader:public AntShaderProgram
{
  float t;
 public:
  AntWaterShader();
  void doUpdate(float time);
};

#endif
