
#include "ag_radio.h"
#include "ag_theme.h"
#include "ag_image.h"




AGRadioGroup::AGRadioGroup():
  sigChanged("changed")
{
}
AGRadioGroup::~AGRadioGroup()
{
}

void AGRadioGroup::eventChange(const std::string &p)
{
  std::set<AGRadio*>::iterator i=mChildren.begin();

  for(;i!=mChildren.end();i++)
    {
      if((*i)->getName()!=p)
	(*i)->deselect();
    }

}

void AGRadioGroup::add(AGRadio *r)
{
  mChildren.insert(r);
}
void AGRadioGroup::erase(AGRadio *r)
{
  mChildren.erase(r);
}









AGRadio::AGRadio(AGWidget *pParent,AGRect pRect,AGRadioGroup *pGroup,std::string pName):
  AGCheckBox(pParent,pRect,pName,"radioButton"),mGroup(pGroup)
{
  if(mGroup)
    mGroup->add(this);
}

AGRadio::~AGRadio()
{
  if(mGroup)
    mGroup->erase(this);
}


bool AGRadio::eventMouseClick(const AGEvent *m)
{
  if(!mChecked)
    {
      mChecked=true;
      mImage->setSurface(getTheme()->getSurface(mType+".checked"));
      if(mGroup)
	mGroup->eventChange(getName());
    }
  return true;
}

void AGRadio::deselect()
{
  mImage->setSurface(getTheme()->getSurface(mType+".normal"));
  mChecked=false;
}
