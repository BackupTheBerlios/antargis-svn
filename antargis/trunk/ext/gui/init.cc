#include <rk_base.h>
#include <rk_debug.h>
#include <ag_layout.h>

AGEXPORT void AG_Init_libantargisgui()
  {
    AGLayout::registerLayouts();
  }

