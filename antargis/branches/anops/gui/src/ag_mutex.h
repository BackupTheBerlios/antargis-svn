#ifndef AG_MUTEX
#define AG_MUTEX

#include <SDL.h>
#include <SDL_thread.h>

class AGMutex
{
 public:
  AGMutex();
  ~AGMutex();

  void enter();
  void leave();
 private:
  SDL_mutex *mut;

};

#endif
