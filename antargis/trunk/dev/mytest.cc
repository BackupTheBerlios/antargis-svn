#include "ant_gl.h"

#include <SDL.h>
#include <iostream>

int main(int argc,char *argv[])
  {
    AntGL gl;
    gl.initVideoMode(800,600);
    gl.clearBuffer();
    gl.swapBuffers();
    SDL_Delay(1000);
    return 0;
  }
