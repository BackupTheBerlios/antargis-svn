#ifndef AG_MENU_H
#define AG_MENU_H

#include "ag_vtiler.h"
#include "ag_gsurface.h"

class AGMenuEvent:public AGEvent
{
 public:
  AGMenuEvent(AGListener *pListener,const std::string &pName);

  std::string getName() const;
 private:
  std::string mName;
};

class AGMenu:public AGVTiler
{
 public:
  AGMenu(AGWidget *pParent,AGPoint pWishPos,const std::string &pName);

  void show(AGPoint pWishPos);

  void addItem(const std::string &pString);
  AGMenu &addMenu(const std::string &pString);

  void draw(const AGRect &r);

  void eventItemSelected(const std::string &pString);
  void eventItemClicked(const std::string &pString);

  AGSignal sigSelected;

 private:
  AGTexture mBg;
  std::string mName;
  AGPoint mWishPos;
};

#endif
