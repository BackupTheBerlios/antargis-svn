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
