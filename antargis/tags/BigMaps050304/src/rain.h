#ifndef __RAIN_H__
#define __RAIN_H__

#include <list>
#include <ag_gsurface.h>

class Rain
{
  std::list<AGPoint> rainSprites;
  std::list<AGPoint> snowSprites;
  AGTexture rainTexture,snowTexture;
  float rainSpeed,snowSpeed;
  Uint32 last;
  int w,h;
  int mAmount;
  float mSnow;
  
  public:
  Rain(int W,int H,int amount);
  virtual ~Rain();
  void addRain(int amount,bool r=false);
  virtual void draw(const AGRect&);
  void move();
};

#endif
