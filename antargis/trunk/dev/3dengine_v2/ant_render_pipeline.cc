#include "ant_render_pipeline.h"

AntRenderPipeline::AntRenderPipeline()
  {

  }
AntRenderPipeline::~AntRenderPipeline()
  {

  }
void AntRenderPipeline::insert(AntRenderPipelet *pPipelet)
  {
    mPipelets.insert(pPipelet);
  }

void AntRenderPipeline::render()
  {
    buildPipeletOrder();
    
    for(std::list<AntRenderPipelet*>::iterator i=mPipeletOrder.begin();i!=mPipeletOrder.end();i++)
      (*i)->doRender();
  }

void AntRenderPipeline::buildPipeletOrder()
  {
    if(mPipelets.size()!=mPipeletOrder.size()) {
      // rebuild order list
      mPipeletOrder.clear();
      std::set<AntRenderPipelet*> builtPipelets;
      
      // while not all pipelets are ordered
      while(mPipelets.size()>mPipeletOrder.size()) {
        std::set<AntRenderPipelet*>::iterator i=mPipelets.begin();
        for(;i!=mPipelets.end();i++)
          {
           if(builtPipelets.find(*i)==builtPipelets.end()) {
             // not yet built - check if prerequisites ok
             std::set<AntRenderPipelet*> pre=(*i)->getPrerequisites();
             std::set<AntRenderPipelet*>::iterator preIter=pre.begin();
             for(;preIter!=pre.end();preIter++)
               {
                 if(builtPipelets.find(*preIter)==builtPipelets.end())
                   break;
               }
             if(preIter==pre.end()) // ok, none found that wasn't built yet, so it's ok to push it
               {
                 builtPipelets.insert(*i);
                 mPipeletOrder.push_back(*i);
               }
             
           }
          }
        
      }
    }
  }

