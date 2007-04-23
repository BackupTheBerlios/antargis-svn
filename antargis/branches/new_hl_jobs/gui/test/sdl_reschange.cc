#include <SDL.h>
#include <assert.h>

int main()
{
  assert(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO)==0);

  assert(SDL_SetVideoMode(800,600,32,SDL_SWSURFACE));

  assert(SDL_SetVideoMode(1024,768,32,SDL_HWPALETTE|SDL_OPENGL|SDL_SWSURFACE));

  return 0;
}
