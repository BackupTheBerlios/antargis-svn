#include "ag_checkbox.h"
#include "ag_image.h"
#include "ag_text.h"
#include "ag_theme.h"

AGCheckBox::AGCheckBox(AGWidget *pParent,AGRect pRect,const std::string &pName,const std::string &pType):
  AGHTiler(pParent,pRect),mName(pName),mChecked(false),mType(pType)
{
  addChild(mImage=new AGImage(this,AGPoint(0,0),getTheme()->getSurface(mType+".normal"),false));
  addChild(new AGText(this,AGPoint(0,0),pName,getTheme()->getFont(std::string("Font.")+mType)));

  adaptHeightFromChildren();
}

bool AGCheckBox::eventMouseClick(const AGEvent *m)
{
  mChecked=!mChecked;

  if(mChecked)
    mImage->setSurface(getTheme()->getSurface(mType+".checked"));
  else
    mImage->setSurface(getTheme()->getSurface(mType+".normal"));
  return true; // eat
}

std::string AGCheckBox::getName() const
{
  return mName;
}
