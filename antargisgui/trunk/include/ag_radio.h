#ifndef AG_RADIO_H
#define AG_RADIO_H

#include "ag_checkbox.h"

class AGRadio;

class AGRadioGroup:public AGMessageObject
{
 public:
  AGRadioGroup();
  virtual ~AGRadioGroup();

  virtual void eventChange(const std::string &p);

  void add(AGRadio *r);
  void erase(AGRadio *r);
  
  AGSignal sigChanged;
 private:
  std::set<AGRadio*> mChildren;
};

class AGRadio:public AGCheckBox
{
 public:
  AGRadio(AGWidget *pParent,AGRect pRect,AGRadioGroup *pGroup,std::string pName);
  virtual ~AGRadio();

  bool eventMouseClick(const AGEvent *m);

  void deselect();

 private:
  AGRadioGroup *mGroup;
};

#endif
