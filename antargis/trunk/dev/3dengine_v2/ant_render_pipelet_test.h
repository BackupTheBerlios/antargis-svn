#ifndef ANT_REDNER_PIPELET_TEST_H
#define ANT_REDNER_PIPELET_TEST_H

#include "ant_render_pipelet.h"

class AntRenderPipeletTest:public AntRenderPipelet
  {
public:
  AntRenderPipeletTest(AntRenderTarget *pRenderTarget);

  void render();
  };

#endif
