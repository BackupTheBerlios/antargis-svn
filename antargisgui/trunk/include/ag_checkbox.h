#ifndef AG_CHECK_BOX_H
#define AG_CHECK_BOX_H

#include "ag_vtiler.h"

class AGImage;

class AGCheckBox:public AGHTiler
{
 public:
  AGCheckBox(AGWidget *pParent,AGRect pRect,const std::string &pName,const std::string &pType="checkBox");

  virtual bool eventMouseClick(const AGEvent *m);

  std::string getName() const;

 protected:
  AGImage *mImage;
 private:
  std::string mName;
 protected:
  bool mChecked;
  std::string mType;
};

#endif
