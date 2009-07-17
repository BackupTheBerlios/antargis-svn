#include <ag_main.h>
#include <rk_debug.h>

#include <list>
#include <string>
#include <string>
#include <map>

#include <SDL.h>

//#include "antargisexternalswig.h"

typedef struct swig_type_info;

typedef swig_type_info*(*CastFunction)(void**);

AGEXPORT std::map<std::string,std::list<CastFunction> > agCastFunctions;

AGEXPORT void AG_Init_libantargisbasic()
  {
    cdebug("SDL_Init...");
    SDL_Init(SDL_INIT_VIDEO);
    cdebug("hasMain?");
    if(!hasMain())
      {
        cdebug("new AGMain..");

        AGMain *main=new AGMain;
        cdebug("new AGMAin!");
      }

    // register main in ruby-var

    // the following can't be done, because getMain is not yet exported to ruby at the point of the flow
    //  AG_NewPointerObj(result,SWIGTYPE_p_AGMain,0);
    //  rb_eval_string("include Libantargisbasic;$agMain=getMain");

    //  #error "DO IT!"

  }

