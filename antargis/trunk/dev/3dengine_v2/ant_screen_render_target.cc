#include "ant_screen_render_target.h"
#include "ant_gl.h"
#include <ag_debug.h>

AntScreenRenderTarget::AntScreenRenderTarget(const AGRect2 &pRect):
  mRect(pRect)
  {

  }

void AntScreenRenderTarget::select()
  {
    CTRACE;
    cdebug("MRECT:"<<mRect);
    glViewport((GLint)mRect.x(),
        (GLint)mRect.y(),
        (GLint)mRect.width(),
        (GLint)mRect.height()
        );
  }

