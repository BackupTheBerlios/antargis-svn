#ifndef AG_SCREEN_H
#define AG_SCREEN_H

#include "ag_widget.h"
#include "ag_gsurface.h"

class AGScreenWidget:public AGWidget
{
 public:
  AGScreenWidget();
  virtual ~AGScreenWidget();

  virtual void draw(const AGRect &r);
 private:
  AGTexture mTexture;
};

#endif
