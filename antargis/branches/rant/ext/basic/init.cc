#include <ag_main.h>
#include <ag_debug.h>

#include <list>
#include <string>
#include <map>

//#include "antargisexternalswig.h"

typedef struct swig_type_info;

typedef swig_type_info*(*CastFunction)(void**);

std::map<std::string,std::list<CastFunction> > agCastFunctions;

AGEXPORT void AG_Init_libantargisbasic()
{
  TRACE;
  if(!hasMain())
    AGMain *main=new AGMain;
}

