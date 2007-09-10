#include <ag_main.h>
#include <ag_debug.h>

#include <list>
#include <string>
#include <map>

//#include "antargisexternalswig.h"

typedef struct swig_type_info;

typedef swig_type_info*(*CastFunction)(void**);

AGEXPORT std::map<std::string,std::list<CastFunction> > agCastFunctions;

AGEXPORT void AG_Init_libantargisbasic()
{
  //  TRACE;
  if(!hasMain())
    AGMain *main=new AGMain;


  // register main in ruby-var

  // the following can't be done, because getMain is not yet exported to ruby at the point of the flow
  //  AG_NewPointerObj(result,SWIGTYPE_p_AGMain,0);
  //  rb_eval_string("include Libantargisbasic;$agMain=getMain");

  //  #error "DO IT!"

}

