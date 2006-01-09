#ifndef AG_FRAME_H
#define AG_FRAME_H

#include "ag_border.h"
#include "ag_widget.h"

class AGFrame:public AGWidget
{
 public:
  AGFrame(AGWidget *pParent,const AGRect &pRect,size_t width); // transparent frame
  AGFrame(AGWidget *pParent,const AGRect &pRect,const AGBorder &pBorder);
  ~AGFrame();

  AGRect getClientRect() const;

  void draw(AGPainter &p);

 private:
  size_t mWidth;
  AGBorder *mBorder;
};

#endif
