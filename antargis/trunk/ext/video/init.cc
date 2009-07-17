#include <rk_debug.h>
#include <ag_main.h>
#include <ag_video.h>

void AGEXPORT AG_Init_libantargisvideo()
  {
    AGVideoManager *video=new AGVideoManager;
    getMain()->setVideo(video);
  }

