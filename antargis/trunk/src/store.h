#ifndef STORE_H
#define STORE_H

#include "mpng.h"

struct Filename
{
  std::string name;
  Filename(std::string filename):name(filename)
  {
  }
};

template<class T,class F>
  T convert(const F &f);

template<class T,class F>
  T convert(const F *f);

template<class T>
std::string convert(const T&t)
{
  return t.toString();
}

template<>
inline std::string convert(const SDL_Surface *ps)
{
  return toPNG(ps);
}

template<>
inline SDL_Surface *convert(const std::string &s)
{
  return fromPNG(s);
}

template<class R>
R convert(Filename f)
{
  return convert<R>(loadFile(f));
}

inline float toFloat(const std::string &s)
{
  return atof(s.c_str());
}

/*template<class T>
std::string toString(const T&t)
{
  return convert<std::string>(t);
}
*/
inline float antRound(float a)
{
  return (int)a;
}


#endif
