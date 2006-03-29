/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_image.h
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

#ifndef AG_IMAGE_H
#define AG_IMAGE_H

#include "ag_widget.h"
#include "ag_texture.h"

class AGImage:public AGWidget
{
 public:
  AGImage(AGWidget *pParent,const AGRect2 &r,AGSurface pSurface,bool pTile,const AGRect2 &pRect=AGRect2(0,0,0,0));
  AGImage(AGWidget *pParent,const AGRect2 &r,AGTexture pTexture,bool pTile,const AGRect2 &pRect=AGRect2(0,0,0,0));
  virtual ~AGImage();

  virtual void draw(AGPainter &p);

  void setSurface(const AGSurface &pSurface);
  void setTexture(const AGTexture &pTexture);

  void useTextures();

  void setCenter(bool c);

 private:
  AGTexture mTexture;
  bool mTile;
  bool mCenter;
  AGRect2 mSrcRect;
};

#endif
