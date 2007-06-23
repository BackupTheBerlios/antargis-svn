#ifndef AG_FRAME_H
#define AG_FRAME_H

#include "ag_border.h"
#include "ag_background.h"
#include "ag_widget.h"

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

class AGFrame:public AGWidget
{
 public:
  AGFrame(AGWidget *pParent,const AGRect2 &pRect,size_t width); // transparent frame
  AGFrame(AGWidget *pParent,const AGRect2 &pRect,const AGBorder &pBorder);
  ~AGFrame();

  void setBackground(const AGBackground &pBg);

  AGRect2 getClientRect() const;

  void draw(AGPainter &p);
  void prepareDraw();

 private:
  size_t mWidth;
  AGBorder *mBorder;
  AGBackground *mBg;
  AGTexture mTexture;
  bool mTextureInited;
  bool mUseTexture;
};

#endif
