#ifndef MESH_2D_DATA
#define MESH_2D_DATA

#include <ag_texture.h>

class Mesh2DData:public AGRubyObject
{
 public:
  Mesh2DData(AGTexture *pTexture);
  
  void mark();

  AGTexture *getTexture();

  AGBox3 bbox() const;

 private:
  AGTexture *mTexture;
};

#endif
