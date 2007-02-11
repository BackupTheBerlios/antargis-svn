#ifndef __ANTARGIS_H__
#define __ANTARGIS_H__
#include "../gui/src/ag_rubyobj.h"
#include "../gui/src/ag_messageobject.h"
#include "../gui/src/ag_serial.h"
#include "../gui/src/ag_aes.h"
#include "../gui/src/ag_singleton.h"
#include "../gui/src/ag_geometry.h"
#include "../gui/src/ag_font.h"
#include "../gui/src/ag_color.h"
#include "../gui/src/ag_local.h"
#include "../gui/src/ag_config.h"
#include "../gui/src/ag_string.h"
#include "../gui/src/ag_string_utf8.h"
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
#include "../gui/src/ag_glscreen.h"
#include "../gui/src/ag_table.h"
#include "../gui/src/ag_texturecache.h"
#include "../gui/src/ag_theme.h"
#include "../gui/src/ag_tools.h"
#include "../gui/src/ag_window.h"
#include "../gui/src/ag_xml.h"
#include "../gui/src/ag_screenwidget.h"
#include "../gui/src/ag_scroller.h"
#include "../gui/src/ag_plugin.h"
#include "../gui/src/ag_destructor.h"
#include "../src/ant_frustum.h"
#include "../src/ant_projection.h"
#include "../src/ant_camera.h"
#include "../src/scenenode.h"
#include "../src/anim_mesh.h"
#include "../src/anim_mesh_data.h"
#include "../src/ant_app.h"
#include "../src/entity.h"
#include "../src/entptr.h"
#include "../src/glsl.h"
#include "../src/height_map.h"
#include "../src/map.h"
#include "../src/mesh_data.h"
#include "../src/mesh.h"
#include "../src/mesh_optimizer.h"
#include "../src/minimap.h"
#include "../src/new_decal.h"
#include "../src/ant_renderer.h"
#include "../src/resource.h"
#include "../src/scene.h"
#include "../src/smoke.h"
#include "../src/terrain.h"
#include "../src/vertex_array.h"
#include "../src/water.h"
#include "../src/path.h"
#include <SDL_keysym.h>
#ifdef SWIG
%include "../gui/src/ag_rubyobj.h"
%include "../gui/src/ag_messageobject.h"
%include "../gui/src/ag_serial.h"
%include "../gui/src/ag_aes.h"
%include "../gui/src/ag_singleton.h"
%include "../gui/src/ag_geometry.h"
%include "../gui/src/ag_font.h"
%include "../gui/src/ag_color.h"
%include "../gui/src/ag_local.h"
%include "../gui/src/ag_config.h"
%include "../gui/src/ag_string.h"
%include "../gui/src/ag_string_utf8.h"
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
%include "../gui/src/ag_glscreen.h"
%include "../gui/src/ag_table.h"
%include "../gui/src/ag_texturecache.h"
%include "../gui/src/ag_theme.h"
%include "../gui/src/ag_tools.h"
%include "../gui/src/ag_window.h"
%include "../gui/src/ag_xml.h"
%include "../gui/src/ag_screenwidget.h"
%include "../gui/src/ag_scroller.h"
%include "../gui/src/ag_plugin.h"
%include "../gui/src/ag_destructor.h"
%include "../src/ant_frustum.h"
%include "../src/ant_projection.h"
%include "../src/ant_camera.h"
%include "../src/scenenode.h"
%include "../src/anim_mesh.h"
%include "../src/anim_mesh_data.h"
%include "../src/ant_app.h"
%include "../src/entity.h"
%include "../src/entptr.h"
%include "../src/glsl.h"
%include "../src/height_map.h"
%include "../src/map.h"
%include "../src/mesh_data.h"
%include "../src/mesh.h"
%include "../src/mesh_optimizer.h"
%include "../src/minimap.h"
%include "../src/new_decal.h"
%include "../src/ant_renderer.h"
%include "../src/resource.h"
%include "../src/scene.h"
%include "../src/smoke.h"
%include "../src/terrain.h"
%include "../src/vertex_array.h"
%include "../src/water.h"
%include "../src/path.h"
%include "/usr/include/SDL/SDL_keysym.h"
#endif
#endif
