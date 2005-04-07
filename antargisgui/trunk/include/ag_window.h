#ifndef AG_WINDOW_H
#define AG_WINDOW_H

#include "ag_table.h"

class AGWindow:public AGTable
{
 public:
  AGWindow(AGWidget *pWidget,const AGRect &pRect,const std::string &pTitle="");

  void addChild(AGWidget *w);

  AGWidget *getClient();

  virtual bool eventMouseButtonDown(const AGEvent *m);
  bool eventDragBy(const AGEvent *event,const AGPoint &pDiff);

  bool tryClose(const char*pName,const AGEvent *m);

  AGRect getClientRect() const;

 private:

  AGWidget *getTitleBar();

  std::string mTitle;
  AGWidget *mClient;
};

#endif
