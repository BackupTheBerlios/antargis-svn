#ifndef __AG_LISTBOX_H
#define __AG_LISTBOX_H

class AGListBox:public AGWidget
{
 public:
  AGListBox(AGWidget *pParent,const AGRect &pRect,int pHeight);

 private:
  int mHeight;
};


#endif
