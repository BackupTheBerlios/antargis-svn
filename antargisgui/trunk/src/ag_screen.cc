#include "ag_screen.h"
#include "ag_surface.h"
#include "ag_theme.h"
#include "ag_debug.h"


AGScreenWidget::AGScreenWidget():
  AGWidget(0,getScreen().getRect())
{
  mTexture=AGTexture(getTheme()->getSurface("backgroundSurface.screen"));
}
AGScreenWidget::~AGScreenWidget()
{
}

void AGScreenWidget::draw(const AGRect &r)
{
  getScreen().tile(mTexture);
}
