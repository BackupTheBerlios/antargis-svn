#ifndef AG_GL_H
#define AG_GL_H

#include <GLee.h>

// patch for mac os
#ifdef GL_GLEXT_VERSION
#undef GL_GLEXT_VERSION
#endif

#include <SDL_opengl.h>
#include <GL/glu.h>

#endif
