#include <ag_gui.h>


void initGUIEngine()
{
#warning "include check for activated video-engine"

  SDL_EnableUNICODE(1);
  AGLayout::registerLayouts(); // register widget types for xml-loading
}
