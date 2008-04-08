#include "ant_sdl.h"

#include <iostream>
#include <SDL.h>


AntSDL *AntSDL::myInstance=0;

AntSDL::AntSDL()
  {
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO)<0)
      {
        std::cerr<<"SDL could not be initialized!"<<std::endl;
        exit(1);
      }
    SDL_EnableUNICODE(1);
    std::cout<<"SDL initialized!"<<std::endl;
  }

AntSDL *AntSDL::getInstance()
  {
    if(!myInstance)
      myInstance=new AntSDL();
    return myInstance;
  }

void needsSDL()
  {
    std::cout<<"NEEDS::"<<std::endl;
    AntSDL::getInstance();
    std::cout<<"NEEDS::"<<std::endl;
  }
