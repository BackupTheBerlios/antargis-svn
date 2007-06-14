#include <ag_debug.h>
#include <ag_main.h>
#include <ag_rand.h>


void AG_Init_libantargismath()
{
	TRACE;
	getMain()->setRand(new AGRandomizer(""));
}
