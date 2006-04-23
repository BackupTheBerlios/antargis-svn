#ifndef __ANTARGIS_H__
#define __ANTARGIS_H__
#include "../gui/src/ag_rubyobj.h"
#include "../gui/src/ag_messageobject.h"
#include "../gui/src/ag_geometry.h"
#include "../gui/src/ag_font.h"
#include "../gui/src/ag_color.h"
#include "../gui/src/ag_local.h"
#include "../gui/src/ag_config.h"
#include "../gui/src/ag_widget.h"
#include "../gui/src/ag_colorbutton.h"
#include "../gui/src/ag_glwidget.h"
#include "../gui/src/ag_xml.h"
#include "../gui/src/ag_layout.h"
#include "../gui/src/ag_dialog.h"
#include "../gui/src/ag_mutex.h"
#include "../gui/src/ag_application.h"
#include "../gui/src/ag_background.h"
#include "../gui/src/ag_border.h"
#include "../gui/src/ag_button.h"
#include "../gui/src/ag_text.h"
#include "../gui/src/ag_caption.h"
#include "../gui/src/ag_checkbox.h"
#include "../gui/src/ag_combo.h"
#include "../gui/src/ag_edit.h"
#include "../gui/src/ag_fontengine.h"
#include "../gui/src/ag_fs.h"
#include "../gui/src/ag_painttarget.h"
#include "../gui/src/ag_surface.h"
#include "../gui/src/ag_texture.h"
#include "../gui/src/ag_surfacemanager.h"
#include "../gui/src/ag_image.h"
#include "../gui/src/ag_layoutfactory.h"
#include "../gui/src/ag_listbox.h"
#include "../gui/src/ag_list.h"
#include "../gui/src/ag_main.h"
#include "../gui/src/ag_menu.h"
#include "../gui/src/ag_menuitem.h"
#include "../gui/src/ag_mixer.h"
#include "../gui/src/ag_mutex.h"
#include "../gui/src/ag_painter.h"
#include "../gui/src/ag_png.h"
#include "../gui/src/ag_radio.h"
#include "../gui/src/ag_screen.h"
#include "../gui/src/ag_table.h"
#include "../gui/src/ag_texturecache.h"
#include "../gui/src/ag_theme.h"
#include "../gui/src/ag_tools.h"
#include "../gui/src/ag_window.h"
#include "../gui/src/ag_xml.h"
#include "ant_frustum.h"
#include "ant_projection.h"
#include "scenenode.h"
#include "anim_mesh.h"
#include "anim_mesh_data.h"
#include "ant_app.h"
#include "ant_serial.h"
#include "entity.h"
#include "entptr.h"
#include "glsl.h"
#include "height_map.h"
#include "map.h"
#include "mesh_data.h"
#include "mesh.h"
#include "mesh_optimizer.h"
#include "minimap.h"
#include "new_decal.h"
#include "ant_renderer.h"
#include "resource.h"
#include "scene.h"
#include "smoke.h"
#include "terrain.h"
#include "vertex_array.h"
#include "water.h"
#include <SDL_keysym.h>
#ifdef SWIG
%include "../gui/src/ag_rubyobj.h"
%include "../gui/src/ag_messageobject.h"
%include "../gui/src/ag_geometry.h"
%include "../gui/src/ag_font.h"
%include "../gui/src/ag_color.h"
%include "../gui/src/ag_local.h"
%include "../gui/src/ag_config.h"
%include "../gui/src/ag_widget.h"
%include "../gui/src/ag_colorbutton.h"
%include "../gui/src/ag_glwidget.h"
%include "../gui/src/ag_xml.h"
%include "../gui/src/ag_layout.h"
%include "../gui/src/ag_dialog.h"
%include "../gui/src/ag_mutex.h"
%include "../gui/src/ag_application.h"
%include "../gui/src/ag_background.h"
%include "../gui/src/ag_border.h"
%include "../gui/src/ag_button.h"
%include "../gui/src/ag_text.h"
%include "../gui/src/ag_caption.h"
%include "../gui/src/ag_checkbox.h"
%include "../gui/src/ag_combo.h"
%include "../gui/src/ag_edit.h"
%include "../gui/src/ag_fontengine.h"
%include "../gui/src/ag_fs.h"
%include "../gui/src/ag_painttarget.h"
%include "../gui/src/ag_surface.h"
%include "../gui/src/ag_texture.h"
%include "../gui/src/ag_surfacemanager.h"
%include "../gui/src/ag_image.h"
%include "../gui/src/ag_layoutfactory.h"
%include "../gui/src/ag_listbox.h"
%include "../gui/src/ag_list.h"
%include "../gui/src/ag_main.h"
%include "../gui/src/ag_menu.h"
%include "../gui/src/ag_menuitem.h"
%include "../gui/src/ag_mixer.h"
%include "../gui/src/ag_mutex.h"
%include "../gui/src/ag_painter.h"
%include "../gui/src/ag_png.h"
%include "../gui/src/ag_radio.h"
%include "../gui/src/ag_screen.h"
%include "../gui/src/ag_table.h"
%include "../gui/src/ag_texturecache.h"
%include "../gui/src/ag_theme.h"
%include "../gui/src/ag_tools.h"
%include "../gui/src/ag_window.h"
%include "../gui/src/ag_xml.h"
%include "ant_frustum.h"
%include "ant_projection.h"
%include "scenenode.h"
%include "anim_mesh.h"
%include "anim_mesh_data.h"
%include "ant_app.h"
%include "ant_serial.h"
%include "entity.h"
%include "entptr.h"
%include "glsl.h"
%include "height_map.h"
%include "map.h"
%include "mesh_data.h"
%include "mesh.h"
%include "mesh_optimizer.h"
%include "minimap.h"
%include "new_decal.h"
%include "ant_renderer.h"
%include "resource.h"
%include "scene.h"
%include "smoke.h"
%include "terrain.h"
%include "vertex_array.h"
%include "water.h"
%include "/usr/include/SDL/SDL_keysym.h"
#endif
#endif
