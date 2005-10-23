/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * antargis.h
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

#ifndef __ANTARGIS_H__
#define __ANTARGIS_H__

#include "map.h"
//#include "minimap.h"
//#include "voxel_gen.h"
#include "scenenode.h"
#include "mesh.h"
#include "scene.h"
#include "antapp.h"
#include "entity.h"
#include "terrain.h"
#include "tree.h"
//#include "ant_man.h"
//#include "ant_geometry.h"
#ifdef SWIG
%include "map.h"
 //%include "minimap.h"
%include "scenenode.h"
%include "mesh.h"
%include "scene.h"
%include "antapp.h"
%include "entity.h"
%include "terrain.h"
%include "tree.h"
 //%include "ant_man.h"
 //%include "ant_geometry.h"

#endif

#endif
