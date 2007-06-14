#ifndef AG_UTF8_H
#define AG_UTF8_H

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#include <string>
#include <vector>
#include <SDL.h>

size_t agFirstCharSize(const std::string &s,size_t pos=0);
size_t agStringLength(const std::string &s);
std::string agSubStr(const std::string &s,size_t from,size_t len);
std::string unicode2Utf8(Uint16 unicode);

#endif
