#ifndef ANT_SDL_H
#define ANT_SDL_H

class AntSDL
  {
public:
  static AntSDL *getInstance();
private:
  AntSDL();
  
  static AntSDL *myInstance;
  };

void needsSDL();

#endif
