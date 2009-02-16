#include <rk_debug.h>
#include <ag_main.h>
#include <ag_video.h>

void AGEXPORT AG_Init_libantargisvideo()
  {
    TRACE;
    AGVideoManager *video=new AGVideoManager;
    getMain()->setVideo(video);
  }

