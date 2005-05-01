/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_tree.h
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

#ifndef ANT_TREE_H
#define ANT_TREE_H

#include "decast.h"
#include "quadtree.h"
#include "voxel_gen.h"
#include "tree.h"

#include "ag_xml.h"
#include <ag_fs.h>

#include <set>
#include <vector>
#include <algorithm>

#include "entity.h"

class AntTree:public AntEntity
  {
    int typeID;
  public:
    AntTree();
    AntTree(const Pos2D &p,int ID);
    VoxelImage *getSurface() const;

    virtual std::string xmlName() const;

    virtual void saveXML(xmlpp::Node &node) const;
    virtual void loadXML(const xmlpp::Node &node);
  };

#endif
