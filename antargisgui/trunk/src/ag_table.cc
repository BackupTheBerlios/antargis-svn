#include "ag_table.h"
#include "ag_debug.h"

AGTable::AGTable(AGWidget *pWidget,const AGRect &pRect):
  AGWidget(pWidget,pRect),
  w(0),h(0),xw(0),yw(0),mInserted(false)
{
}

void AGTable::addFixedColumn(float size)
{
  cdebug(size);
  if(mInserted)
    return;
  cols.push_back(std::make_pair(size,true));
  w++;
}
void AGTable::addFixedRow(float size)
{
  cdebug(size);
  if(mInserted)
    return;
  rows.push_back(std::make_pair(size,true));
  h++;
}

void AGTable::addColumn(float weight)
{
  cdebug(weight);
  if(mInserted || weight<=0.0f)
    return;
  cols.push_back(std::make_pair(weight,false));
  w++;
  xw+=weight;
}

void AGTable::addRow(float weight)
{
  cdebug(weight);
  if(mInserted || weight<=0.0f)
    return;
  rows.push_back(std::make_pair(weight,false));
  h++;
  yw+=weight;
}

void AGTable::addChild(int px,int py,AGWidget *pWidget)
{
  if(px>=w || py>=h || px<0 || py<0)
    {
      cdebug("wrong input position");
      return;
    }

  if(!mInserted)
    children=std::vector<AGWidget*>(w*h,(AGWidget*)0);

  if(children[px+py*w])
    cdebug("Children "<<px<<","<<py<<" already set!");

  mInserted=true;
  children[px+py*w]=pWidget;
  AGWidget::addChild(pWidget);
}

/*
void AGTable::getWidthFromChildren()
{
}
void AGTable::getHeightFromChildren()
{
}
*/
void AGTable::arrange()
{
  int mx,my;

  // first get the fixed sizes
  int fx,fy;
  int mfx,mfy; // max
  fx=fy=0;
  mfx=mfy=0;
  
  // in y dir
  for(mx=0;mx<w;mx++)
    {
      fy=0;
      //      if(cols[mx]==0.0)
	for(my=0;my<h;my++)
	  {
	    AGWidget *wd=children[mx+my*w];
	    if(wd)
	      {
		if(rows[my].second)
		  fy+=(int)(rows[my].first);
	      }
	  }
      mfy=std::max(mfy,fy);
    }

  // x dir
  for(my=0;my<h;my++)
    {
      fx=0;
      //      if(rows[my]==0.0) // only check fixed
	for(mx=0;mx<w;mx++)
	  {
	    AGWidget *wd=children[mx+my*w];
	    if(wd)
	      {
		if(cols[mx].second)
		  {
		    fx+=(int)(cols[mx].first);
		  }
	      }
	  }
      mfx=std::max(mfx,fx);
    }

  /*  ccdebug("mfx:"<<mfx);
  ccdebug("mfy:"<<mfy);
  ccdebug("width:"<<width());*/
  // assign width's and height's for all non-fixed
  for(mx=0;mx<w;mx++)
    for(my=0;my<h;my++)
      {
	if(!cols[mx].second)
	  {
	    AGWidget *wd=children[mx+my*w];
	    if(wd)
	      {
		wd->setWidth((int)((width()-mfx)*cols[mx].first/xw));
	      }
	  }
	if(!rows[my].second)
	  {
	    AGWidget *wd=children[mx+my*w];
	    if(wd)
	      {
		wd->setHeight((int)((height()-mfy)*rows[my].first/yw));
	      }
	  }
      }

  // assign positions
  // first x 
  int ax=0;
  for(mx=0;mx<w;mx++)
    {
      int maxx=0;
      for(my=0;my<h;my++)
	{
	  AGWidget *wd=children[mx+my*w];
	  if(wd)
	    {
	      wd->setLeft(ax);

	      maxx=std::max(maxx,wd->width());
	    }
	}
      ax+=maxx;
    }

  // now y
  int ay=0;
  for(my=0;my<h;my++)
    {
      int maxy=0;
      for(mx=0;mx<w;mx++)
	{
	  AGWidget *wd=children[mx+my*w];
	  if(wd)
	    {
	      wd->setTop(ay);

	      maxy=std::max(maxy,wd->height());
	    }
	}
      ay+=maxy;
    }
  
}


void AGTable::setWidth(int w)
{
  cdebug(w);
  AGWidget::setWidth(w);
  arrange();
}
void AGTable::setHeight(int w)
{
  AGWidget::setHeight(w);
  arrange();
}
