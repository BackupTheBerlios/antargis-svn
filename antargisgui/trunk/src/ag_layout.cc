/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_layout.cc
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

#include "ag_layout.h"
#include "ag_xml.h"
#include "ag_tools.h"

#include "ag_button.h"
#include "ag_window.h"

AGLayout::AGLayout(AGWidget *pgParent,const std::string &pXMLData):
  AGWidget(pgParent,AGRect(0,0,0,0))
{
  xmlpp::Document p;
  p.parse_memory(pXMLData);

  AGRect geom;
  std::string geomS=p.root().get("geometry");
  if(geomS.length())
    geom=AGRect(geomS);

  setRect(geom);

  parseChildren(this,p.root());

}

AGWidget *AGLayout::parseNode(AGWidget *pParent,const xmlpp::Node &pNode)
{
  std::string n=pNode.getName();

  AGRect geom=getGeometry(pParent,pNode);

  AGWidget *w=0;

  std::cout<<"huhu"<<std::endl;
  cdebug("n:"<<n);
  
  if(n=="button")
    {
      std::string caption=pNode.get("caption");
      w=new AGButton(pParent,geom,caption);
    }
  else if(n=="window")
    {
      std::string title=pNode.get("title");
      w=new AGWindow(pParent,geom,title);
    }
  else if(n=="table")
    {
      w=parseTable(pParent,pNode,geom);
      pParent->addChild(w);
      w=0;
    }
    

  parseChildren(w,pNode);

  return w;
}

AGRect AGLayout::getGeometry(AGWidget *pParent,const xmlpp::Node &pNode)
{
  AGRect geom=pParent->getClientRect();
  std::string geomS=pNode.get("geometry");
  if(geomS.length())
    geom=AGRect(geomS);
  return geom;
}

void AGLayout::parseChildren(AGWidget *pParent,const xmlpp::Node &pNode)
{
  if(pParent)
    {
      xmlpp::Node::const_iterator i=pNode.begin();
      for(;i!=pNode.end();i++)
	{
	  AGWidget *w=parseNode(pParent,*i);
	  if(w)
	    pParent->addChild(w);
	}
    }
}

AGTable *AGLayout::parseTable(AGWidget *pParent,const xmlpp::Node &pNode,const AGRect &geom)
{
  TRACE;
  AGTable *t;
  t=new AGTable(pParent,geom);

  int w,h;
  w=toInt(pNode.get("cols"));
  h=toInt(pNode.get("rows"));

  std::vector<std::pair<float,bool> > rows(h);
  std::vector<std::pair<float,bool> > cols(w);

  // parse rows/cols info
  xmlpp::Node::const_iterator i=pNode.begin();
  for(;i!=pNode.end();i++)
    {
      if((*i)->getName()=="colsize")
	{
	  int id=toInt((*i)->get("col"));
	  assert(id>=0 && id<w);
	  cdebug("id:"<<id);
	  std::string s=(*i)->get("fixed");
	  cdebug("fixed:"<<s);
	  if(s.length())
	    cols[id]=std::make_pair(toFloat(s),true);
	  else
	    {
	      s=(*i)->get("relative");
	      cdebug("relative:"<<s);
	      cols[id]=std::make_pair(toFloat(s),false);
	    }
	}
      else if((*i)->getName()=="rowsize")
	{
	  int id=toInt((*i)->get("row"));
	  assert(id>=0 && id<h);

	  std::string s=(*i)->get("fixed");
	  if(s.length())
	    rows[id]=std::make_pair(toFloat(s),true);
	  else
	    {
	      s=(*i)->get("relative");
	      rows[id]=std::make_pair(toFloat(s),false);
	    }
	}
    }

  for(int k=0;k<w;k++)
    {
      cdebug("cols:"<<cols[k].first<<"/////"<<cols[k].second);
      if(cols[k].second)
	t->addFixedColumn(cols[k].first);
      else
	t->addColumn(cols[k].first);
    }
  for(int k=0;k<h;k++)
    {
      if(rows[k].second)
	t->addFixedRow(rows[k].first);
      else
	t->addRow(rows[k].first);
    }

  // now get children

  i=pNode.begin();
  for(;i!=pNode.end();i++)
    {
      AGWidget *w=parseNode(this,*i);
      if(w)
	{
	  int col=toInt((*i)->get("col"));
	  int row=toInt((*i)->get("row"));
	  cdebug(col<<"/"<<row);
	  t->addChild(col,row,w);
	}
    }
  t->arrange();
  return t;
}
