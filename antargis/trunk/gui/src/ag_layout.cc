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
#include "ag_theme.h"
#include "ag_listbox.h"
#include "ag_image.h"
#include "ag_frame.h"

#include "ag_button.h"
#include "ag_edit.h"
#include "ag_window.h"
#include "ag_text.h"
#include "ag_main.h"

#include "ag_layoutfactory.h"


AGLayout::AGLayout(AGWidget *pgParent,const std::string &pXMLData):
  AGWidget(pgParent,AGRect(0,0,0,0))
{
  xmlpp::Document p;
  p.parseMemory(pXMLData);

  AGRect geom;
  std::string geomS=p.root().get("geometry");
  if(geomS.length())
    geom=AGRect(geomS);
  else if(pgParent)
    geom=pgParent->getRect().origin();
  else
    geom=AGRect(0,0,getMain()->width(),getMain()->height());

  setRect(geom);
  setName(p.root().get("name"));

  parseChildren(this,p.root());


      cdebug(mTabIndices.size());
  if(mTabIndices.size())
    {
      CTRACE;
      cdebug(mTabIndices.size());
      cdebug("GAINFOCUS::::::::::::");
      AGWidget *w=mTabIndices.begin()->second;
      cdebug(w->getName());
      w->gainCompleteFocus(); // is ok, because till here layout isn't inserted into screen yet
      //      assert(w->hasFocus());
    }
}

void AGLayout::addTabIndex(int i,AGWidget *pWidget)
{
  mTabIndices[i]=pWidget;
}


AGLayout *getLayout(AGWidget *pWidget)
{
  AGLayout *l=dynamic_cast<AGLayout*>(pWidget);
  if(l)
    return l;
  else if(pWidget->getParent())
    return getLayout(pWidget->getParent());
  return 0;
}


AGWidget *parseNode(AGWidget *pParent,const xmlpp::Node &pNode)
{
  std::string n=pNode.getName();

  AGRect geom=getLayoutGeometry(pParent,pNode);

  AGWidget *w=0;

  cdebug("n:"<<n);

  w=getLayoutFactory()->create(pParent,geom,pNode);

  if(!w)
    {
      //   cdebug("CREATION FAILED!!"<<n);
    }

  //  cdebug("w:"<<w);
  //cdebug("nodename:"<<pNode.get("name"));
  if(w!=0 && pNode.get("name").length())
    {
      w->setName(pNode.get("name"));
    }

  if(w!=0 && pNode.get("tabindex").length())
    {
      AGLayout *l=getLayout(pParent);
      if(l)
	{
	  l->addTabIndex(toInt(pNode.get("tabindex")),w);
	}
      else
	cdebug("ERRRRRRRRRRRRRRRRRRRRRRROR");

    }

  parseChildren(w,pNode);

  return w;
}

AGRect getLayoutGeometry(AGWidget *pParent,const xmlpp::Node &pNode)
{
  //  TRACE;
  AGRect geom=pParent->getClientRect();
  AGTable *t=dynamic_cast<AGTable*>(pParent);
  //  cdebug(pNode.getName());
  //  cdebug(geom);
  if(t)
    if(pNode.get("col")!="" && pNode.get("row")!="")
      {
	int col=toInt(pNode.get("col"));
	int row=toInt(pNode.get("row"));
	
	geom=t->getClientRect(col,row);
	//	assert(pParent->getRect().origin().contains(geom));
	//	cdebug("col:"<<col<<" row:"<<row<<":"<<geom);
	assert(pParent->getRect().origin().contains(geom));
      }
  

  std::string geomS=pNode.get("geometry");
  if(geomS.length())
    geom=AGRect(geomS);
  return geom;
}

void parseChildren(AGWidget *pParent,const xmlpp::Node &pNode)
{
  if(pParent)
    {
      xmlpp::Node::const_iterator i=pNode.begin();
      for(;i!=pNode.end();i++)
	{
	  AGWidget *w=parseNode(pParent,**i);
	  if(w)
	    pParent->addChild(w);
	}
    }
}


// Layout-Factories
class AGButtonLayoutCreator:public AGLayoutCreator
{
 public:
  REGISTER_COMPONENT(Button,"button")
  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    AGButton *b;
    std::string caption=pNode.get("caption");
    b=new AGButton(pParent,pRect,caption);
    
    std::string captionImage=pNode.get("caption-image");
    if(captionImage.length())
      b->setSurface(getSurfaceManager()->loadSurface(captionImage),false);
    if(pNode.get("enabled")=="false")
      b->setEnabled(false);

    b->setTheme(pNode.get("theme"));
    return b;
  }
};


class AGTableLayoutCreator:public AGLayoutCreator
{
 public:
  REGISTER_COMPONENT(Table,"table")


  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    AGWidget *w=parseTable(pParent,pNode,pRect);
    pParent->addChild(w);
    w=0;
    return w;
  }

AGTable *parseTable(AGWidget *pParent,const xmlpp::Node &pNode,const AGRect &geom)
{
  TRACE;
  AGTable *t;
  t=new AGTable(pParent,geom);

  int w,h;
  w=toInt(pNode.get("cols"));
  h=toInt(pNode.get("rows"));

  t->setName(pNode.get("name"));

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
      if(cols[k].first==0.0f) // not inited
	t->addColumn(1.0f);
      else if(cols[k].second)
	t->addFixedColumn(cols[k].first);
      else
	t->addColumn(cols[k].first);
    }
  for(int k=0;k<h;k++)
    {
      if(rows[k].first==0.0f) // not inited
	t->addRow(1.0f);
      else if(rows[k].second)
	t->addFixedRow(rows[k].first);
      else
	t->addRow(rows[k].first);
    }

  // now get children

  i=pNode.begin();
  for(;i!=pNode.end();i++)
    {
      AGWidget *w=parseNode(t,**i);
      if(w)
	{
	  int col=toInt((*i)->get("col"));
	  int row=toInt((*i)->get("row"));
	  cdebug(col<<"/"<<row);
	  t->addChild(col,row,w);
	}
    }
  //  t->arrange();
  return t;
}


};

class AGWindowLayoutCreator:public AGLayoutCreator
{
 public:
  REGISTER_COMPONENT(Window,"window")
  
  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
      std::string title=pNode.get("title");
      std::string theme=pNode.get("theme");
      
      AGWidget *w=new AGWindow(pParent,pRect,title,theme);
      return w;
  }
};

IMPLEMENT_COMPONENT_FACTORY(Table);
IMPLEMENT_COMPONENT_FACTORY(Button);
IMPLEMENT_COMPONENT_FACTORY(Window);


// AGText creator
class AGTextLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(Text,"text")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    std::string text=pNode.get("caption");
    
    //    AGWidget *w=new AGText(pParent,pRect,text,font);
    AGEdit *w=new AGEdit(pParent,pRect);
    text=replace(text,"\\n","\n");
    w->setText(text);
    std::string fontname=pNode.get("font");
    if(fontname=="")
      fontname="text.font";

    AGFont font;
    font=getTheme()->getFont(fontname);
    w->setFont(font);
    
    if(pNode.get("align")=="left")
      w->setAlign(EDIT_LEFT);
    if(pNode.get("align")=="right")
      w->setAlign(EDIT_RIGHT);
    if(pNode.get("align")=="center")
      w->setAlign(EDIT_CENTER);

    if(pNode.get("valign")=="top")
      w->setVAlign(EDIT_TOP);
    if(pNode.get("valign")=="bottom")
      w->setVAlign(EDIT_BOTTOM);
    if(pNode.get("valign")=="center")
      w->setVAlign(EDIT_VCENTER);

    //    w->setAlign(EDIT_CENTER);
    w->setMutable(false);
    w->setBackground(false);

    return w;
  }
};
IMPLEMENT_COMPONENT_FACTORY(Text);


// AGText creator
class AGEditLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(Edit,"edit")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    CTRACE;
    std::string text=pNode.get("text");
    bool multi=pNode.get("multi")=="true";
    
    //    AGWidget *w=new AGText(pParent,pRect,text,font);
    AGEdit *w=new AGEdit(pParent,pRect);
    w->setText(text);
    if(pNode.get("font")!="")
    {
      AGFont font;
      font=getTheme()->getFont(pNode.get("font"));
      w->setFont(font);
    }
    //    w->setAlign(EDIT_CENTER);
    w->setMutable(true);//false);
    w->setBackground(true);//false);
    w->setMulti(multi);
    if(!multi)
      w->setVAlign(EDIT_VCENTER);

    return w;
  }
};
IMPLEMENT_COMPONENT_FACTORY(Edit);



// AGListBox creator
class AGListBoxLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(ListBox,"listBox")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    CTRACE;
    AGListBox *l=new AGListBox(pParent,pRect);

    return l;
  }
};
IMPLEMENT_COMPONENT_FACTORY(ListBox);

// AGLayout creator
class AGLayoutLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(Layout,"layout")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    CTRACE;
    std::string filename=pNode.get("filename");
    AGWidget *w=new AGWidget(pParent,pRect);
    AGLayout *l=new AGLayout(w,loadFile(filename));//pRect);
    w->addChild(l);

    return w;
  }
};
IMPLEMENT_COMPONENT_FACTORY(Layout);


// AGLayout creator
class AGImageLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(Image,"image")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    CTRACE;
    std::string filename=pNode.get("filename");

    AGSurface s(0,0);
    if(filename.length())
      s=getSurfaceManager()->loadSurface(filename);

    AGRect r=pRect;
    if(pNode.get("x").length())
      r.x=toInt(pNode.get("x"));
    if(pNode.get("y").length())
      r.y=toInt(pNode.get("y"));


    AGWidget *w=new AGImage(pParent,r,s,false,pRect);

    return w;
  }
};
IMPLEMENT_COMPONENT_FACTORY(Image);


// AGLayout creator
class AGFrameLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(Frame,"frame")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    CTRACE;
    std::string border=pNode.get("border");
    size_t width=toInt(pNode.get("width"));

    AGWidget *w;
    if(border.length())
      w=new AGFrame(pParent,pRect,AGBorder(border));
    else
      w=new AGFrame(pParent,pRect,width);
    
    return w;
  }
};
IMPLEMENT_COMPONENT_FACTORY(Frame);


// AGLayout creator
class AGCellLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(Cell,"cell")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    return new AGWidget(pParent,pRect);
  }
};
IMPLEMENT_COMPONENT_FACTORY(Cell);
