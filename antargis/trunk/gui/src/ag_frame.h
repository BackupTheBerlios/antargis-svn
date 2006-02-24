#ifndef AG_FRAME_H
#define AG_FRAME_H

#include "ag_border.h"
#include "ag_widget.h"

class AGFrame:public AGWidget
{
 public:
  AGFrame(AGWidget *pParent,const AGRect2 &pRect,size_t width); // transparent frame
  AGFrame(AGWidget *pParent,const AGRect2 &pRect,const AGBorder &pBorder);
  ~AGFrame();

  AGRect2 getClientRect() const;

  void draw(AGPainter &p);
  void prepareDraw();

 private:
  size_t mWidth;
  AGBorder *mBorder;
  AGTexture mTexture;
  bool mTextureInited;
  bool mUseTexture;
};

#endif
