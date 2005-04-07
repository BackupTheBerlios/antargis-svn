#ifndef AG_TABLE_H
#define AG_TABLE_H

#include "ag_widget.h"

#include <vector>

/**
   Description:
   At first you can adjust rows/columns - then you can insert some children.
   after that rows and columns are fixed 
*/

class AGTable:public AGWidget
{
 public:
  AGTable(AGWidget *pWidget,const AGRect &pRect);

  void addFixedColumn(float size);
  void addFixedRow(float size);

  void addColumn(float weight);
  void addRow(float weight);

  void addChild(int x,int y,AGWidget *pWidget);

  //  void getWidthFromChildren();
  //  void getHeightFromChildren();

  void arrange();

  virtual void setWidth(int w);
  virtual void setHeight(int w);

 private:
  int w,h;
  float xw,yw; // weights

  std::vector<std::pair<float,bool> > rows,cols;

  std::vector<AGWidget*> children;

  bool mInserted;
};

#endif
