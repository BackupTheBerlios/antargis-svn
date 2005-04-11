/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * rain.cpp
 * by David Kamphausen (david.kamphausen@web.de)
 *
 * The "Antargis" project, including all files needed to compile it,
 * is free software; you can redistribute it and/or use it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 */

#include "rain.h"

Rain::Rain(int W,int H,int amount):w(W),h(H),mAmount(amount)
{
  AGSurface s=getScreen().loadSurface("data/raindrop.png");
  rainTexture=AGTexture(s);
  s=getScreen().loadSurface("data/snow.png");
  snowTexture=AGTexture(s);

  mSnow=0.0;

  last=SDL_GetTicks();

  rainSpeed=250;
  snowSpeed=150;

  addRain(amount,true);

}
Rain::~Rain()
{}

void Rain::addRain(int amount,bool r)
{
  if(amount>30)
    r=true;
  for(int i=0;i<amount;i++)
    {
      if(rand()%100<100*mSnow)
        {
          // snow
          if(r)
            snowSprites.push_back(AGPoint(rand()%w,rand()%h));
          else if(rand()%2)
            snowSprites.push_back(AGPoint(rand()%w,-16));
          else
            snowSprites.push_back(AGPoint(-16,rand()%h));
        }
      else
        {
          // rain
          if(r)
            rainSprites.push_back(AGPoint(rand()%w,rand()%h));
          else if(rand()%2)
            rainSprites.push_back(AGPoint(rand()%w,-16));
          else
            rainSprites.push_back(AGPoint(-16,rand()%h));
        }
    }
}

void Rain::draw(const AGRect&)
{
  move();
  std::list<AGPoint>::iterator i=rainSprites.begin();
  for(;i!=rainSprites.end();i++)
    {
      getScreen().blit(rainTexture,AGRect(i->x,i->y,16,16));
    }

  i=snowSprites.begin();
  for(;i!=snowSprites.end();i++)
    {
      getScreen().blit(snowTexture,AGRect(i->x,i->y,16,16));
    }
}
void Rain::move()
{
  Uint32 now=SDL_GetTicks();
  Uint32 diff=now-last;

  std::list<AGPoint>::iterator i=rainSprites.begin();
  for(;i!=rainSprites.end();)
    {
      Uint32 add
        =(Uint32)(rainSpeed*diff*0.001);
      i->x+=add
            ;
      i->y+=add
            ;

      if(i->x>w || i->y>h)
        {
          // delete and add new
          i=rainSprites.erase(i);
        }
      else
        i++;
    }

  i=snowSprites.begin();
  for(;i!=snowSprites.end();)
    {
      Uint32 add
        =(Uint32)(snowSpeed*diff*0.001);
      i->x+=add
            ;
      i->y+=add
            ;

      if(i->x>w || i->y>h)
        {
          // delete and add new
          i=snowSprites.erase(i);
        }
      else
        i++;
    }

  size_t s=snowSprites.size()+rainSprites.size();
  if((long)s<mAmount)
    addRain(mAmount-s);


  last=now;
}
