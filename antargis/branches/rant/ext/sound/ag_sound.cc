#include <ag_sound.h>

#include <map>
#include <string>

#include <SDL_mixer.h>

extern std::map<std::string,Mix_Chunk*> mSounds;

#warning FIXME: cleanup stored sounds ???
