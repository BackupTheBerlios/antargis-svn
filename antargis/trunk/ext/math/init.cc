#include <rk_debug.h>
#include <ag_main.h>
#include <ag_rand.h>


void AGEXPORT AG_Init_libantargismath()
  {
    TRACE;
    AGRandomizer *r=new AGRandomizer("");
    cdebug("r:"<<r);
    AGMain *main=getMain();
    cdebug("main:"<<main);
    main->setRand(r);
  }
