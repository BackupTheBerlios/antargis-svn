#include "ag_background.h"
#include "ag_draw.h"
#include "ag_theme.h"
#include "ag_debug.h"

AGBackground::AGBackground(std::string pThemeName):mSurfaceFlag(false)
{
  AGTheme *theme=getTheme();
  if(theme->hasSurface(pThemeName))
    {
      mSurface=AGTexture(theme->getSurface(pThemeName));
      mSurfaceFlag=true;
    }
  else
    {
      mColors[0]=theme->getColor(std::string("gradientColor1.")+pThemeName);
      mColors[1]=theme->getColor(std::string("gradientColor2.")+pThemeName);
      mColors[2]=theme->getColor(std::string("gradientColor3.")+pThemeName);
      mColors[3]=theme->getColor(std::string("gradientColor4.")+pThemeName);
    }
}

void AGBackground::draw(const AGRect &pScreenRect) const
{
  //  AGSurface s(getScreen());
   if(mSurfaceFlag)
    getScreen().tile(mSurface,pScreenRect);
  else
    AGDraw::drawGradient(&getScreen(),pScreenRect,mColors[0],mColors[1],mColors[2],mColors[3]);
}
