#ifndef AG_MENUITEM_H
#define AG_MENUITEM_H

#include "ag_text.h"
#include "ag_vtiler.h"

class AGMenu;

class AGMenuItem:public AGHTiler
{
 public:
  AGMenuItem(AGWidget *pParent,const std::string &pText);
  virtual ~AGMenuItem();

  void draw(const AGRect &r);

  virtual bool eventMouseEnter();
  virtual bool eventMouseLeave();
  
  bool getSelected() const;
  void unSelect();

  std::string getName() const;

  virtual bool eventMouseClick(const AGEvent *m);

  virtual void eventSelect();
  virtual void eventUnselect();

 private:
  std::string mText;
  bool mMouseOver;
  bool mSelected;
};

class AGSubMenu:public AGMenuItem
{
 public:
  AGSubMenu(AGWidget *pParent,const std::string &ptext);
  virtual ~AGSubMenu();

  AGMenu *getMenu();

  virtual void eventSelect();
  virtual void eventUnselect();

  virtual bool eventMouseClick(const AGEvent *m);

 private:
  void addChild(AGWidget*pWidget);

  AGMenu *mSubMenu;
};

#endif
