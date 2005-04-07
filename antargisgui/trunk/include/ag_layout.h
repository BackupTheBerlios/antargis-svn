/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_layout.h
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

#ifndef AG_LAYOUT_H
#define AG_LAYOUT_H

#include "ag_widget.h"
#include "ag_xml.h"

class AGTable;

class AGLayout:public AGWidget
{
 public:
  AGLayout(AGWidget *pgParent,const std::string &pXMLData);
 private:
  AGWidget *parseNode(AGWidget *pParent,const xmlpp::Node &pNode);
  void parseChildren(AGWidget *pParent,const xmlpp::Node &pNode);

  AGTable *parseTable(AGWidget *pParent,const xmlpp::Node &pNode,const AGRect &geom);
  AGRect getGeometry(AGWidget *pParent,const xmlpp::Node &pNode);

};


#endif
