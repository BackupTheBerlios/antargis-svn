#include <rk_debug.h>
#include <ag_main.h>
#include <ag_rand.h>


void AGEXPORT AG_Init_libantargismath()
  {
    AGRandomizer *r=new AGRandomizer("");
    AGMain *main=getMain();
    main->setRand(r);
  }
