#ifndef MPNG_H
#define MPNG_H

#include <string>

#include <SDL.h>

std::string toPNG(const SDL_Surface *s);
SDL_Surface *fromPNG(std::string s);


#endif
