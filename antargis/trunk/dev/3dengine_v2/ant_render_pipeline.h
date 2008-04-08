#ifndef ANT_RENDER_PIPELINE_H
#define ANT_RENDER_PIPELINE_H

#include <set>
#include <list>

#include "ant_render_pipelet.h"

class AntRenderPipeline
  {
    std::set<AntRenderPipelet*> mPipelets;
    std::list<AntRenderPipelet*> mPipeletOrder;

public:
  AntRenderPipeline();
  ~AntRenderPipeline();
  void insert(AntRenderPipelet *pPipelet);
  
  void render();

private:
  void buildPipeletOrder();
  };

#endif
