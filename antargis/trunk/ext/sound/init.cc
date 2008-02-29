#include <ag_main.h>
#include <ag_rand.h>
#include <ag_debug.h>

extern void initSoundEngine();

AGEXPORT void AG_Init_libantargissound()
  {
    TRACE;
    initSoundEngine();

  }
