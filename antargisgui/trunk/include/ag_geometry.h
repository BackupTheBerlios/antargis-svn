#ifndef AG_GEOMETRY_H
#define AG_GEOMETRY_H

#include <SDL.h>
#include <string>
#include <iostream>

class AGPoint
{
 public:
  int x,y;
  AGPoint(int pX,int pY);
  AGPoint();

  AGPoint operator-(const AGPoint &p) const;
};

class AGRect:public SDL_Rect
{
 public:
  AGRect();
  AGRect(const AGRect &r);
  AGRect(const SDL_Rect &r);
  AGRect(int x,int y,int w,int h);

  /*
  int x() const;
  int y() const;
  int width() const;
  int height() const;
  */
  AGRect intersect(const AGRect &r) const;

  bool isNull() const; // is empty

  AGRect shrink(int amount) const;

  bool contains(const AGPoint &p) const;

  AGRect project(const AGRect &r) const;
  AGPoint project(const AGPoint &r) const;

  AGRect origin() const;
  
  AGPoint getPosition() const;

  std::string toString() const;

  //  SDL_Rect rect() const;

  bool operator==(const AGRect &r) const;
  bool operator!=(const AGRect &r) const;

  AGRect operator+(const AGPoint &p) const;

 private:
  //  SDL_Rect mr;
};

std::ostream &operator<<(std::ostream &o,const SDL_Rect &r);


#endif
