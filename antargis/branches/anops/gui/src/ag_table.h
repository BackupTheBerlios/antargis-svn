/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_table.h
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

#ifndef AG_TABLE_H
#define AG_TABLE_H

#include "ag_widget.h"

#include <vector>

/**
   Description:
   At first you can adjust rows/columns - then you can insert some children.
   after that rows and columns are fixed.

   Please call AGTable::arrange() whenever you changed extends and/or row/column-info.
   
   @todo children should be re-arranged whenever AGTable::arrange() is called!
*/

class AGTable:public AGWidget
{
 public:
  AGTable(AGWidget *pWidget,const AGRect2 &pRect);
  virtual ~AGTable();

  void addFixedColumn(float size);
  void addFixedRow(float size);

  void addColumn(float weight);
  void addRow(float weight);

  void addChild(int x,int y,AGWidget *pWidget);
  AGRect2 getClientRect(int x,int y) const;

  void arrange();

  virtual void setWidth(float w);
  virtual void setHeight(float w);

  size_t getRows() const;
  size_t getColumns() const;

 private:
  int w,h;
  float xw,yw; // weights

  std::vector<std::pair<float,bool> > rows,cols;

  std::vector<AGWidget*> children;

  bool mInserted;
};

#endif
