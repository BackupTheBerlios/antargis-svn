/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * minimap.h
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

#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include <ag_widget.h>
#include <ag_surface.h>
#include <ag_border.h>

#include "map.h"

class AntargisMap;

class MiniMap:public AGWidget//,public MapListener
  {
  public:
    MiniMap(AGWidget *pParent,const AGRect &r,AntargisMap *pMap,const AGRect &pViewRect);
    virtual ~MiniMap();

    virtual void draw(AGPainter &p);

    void update(AGPainter &p);
    void drawEntities(AGPainter &p);

    AGSignal sigMoveMap;

  private:
    AntargisMap *mMap;
    AGTexture mTexture;
    //    AGTexture mBG;
    AGBorder mBorder;
    AGSurface mSurface;
    bool mustUpdate;
    AGRect mViewRect;
  };

#endif
