#include "ant_render_pipelet.h"

AntRenderPipelet::AntRenderPipelet(AntRenderTarget *pRenderTarget):
  mRenderTarget(pRenderTarget)
        {

        }
AntRenderPipelet::~AntRenderPipelet()
  {

  }


void AntRenderPipelet::render()
  {

  }

std::set<AntRenderPipelet*> AntRenderPipelet::getPrerequisites() const
{
  return mPrerequisites;
}

void AntRenderPipelet::addPrerequisite(AntRenderPipelet *pPipelet)
  {
    mPrerequisites.insert(pPipelet);
  }

void AntRenderPipelet::doRender()
  {
    mRenderTarget->select();
    render();
    
  }
