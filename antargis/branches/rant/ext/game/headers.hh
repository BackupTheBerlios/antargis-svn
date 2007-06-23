#ifndef __ANTARGIS_H__
#define __ANTARGIS_H__
#include "ext/basic/ag_base.h"
#include "ext/basic/ag_collector.h"
#include "ext/basic/ag_config.h"
#include "ext/basic/ag_debug.h"
#include "ext/basic/ag_exception.h"
#include "ext/basic/ag_fs.h"
#include "ext/basic/ag_main.h"
#include "ext/basic/ag_profiler.h"
#include "ext/basic/ag_regex.h"
#include "ext/basic/ag_rubyobj.h"
#include "ext/basic/ag_rand_base.h"
#include "ext/basic/ag_string.h"
#include "ext/basic/ag_string_utf8.h"
#include "ext/basic/ag_stringstream.h"
#include "ext/basic/ag_tools.h"
#include "ext/basic/ag_utf8.h"
#include "ext/basic/ag_video_base.h"
#include "ext/basic/ag_xml.h"
#include "ext/basic/ag_plugin.h"
#include "ext/basic/ag_singleton.h"
#include "ext/basic/ag_kill.h"
#include "ext/basic/ag_serial.h"
#include "ext/basic/ag_messageobject.h"
#include "ext/basic/ag_mutex.h"
#include "ext/math/ag_gauss.h"
#include "ext/math/ag_geometry.h"
#include "ext/math/ag_rand.h"
#include "ext/math/ag_serial_vec.h"
#include "ext/math/ant_frustum.h"
#include "ext/math/ag_algebra.h"
#include "ext/video/ag_screen.h"
#include "ext/video/ag_video.h"
#include "ext/video/GLee.h"
#include "ext/video/ag_painttarget.h"
#include "ext/video/ag_color.h"
#include "ext/video/ag_fontengine.h"
#include "ext/video/ag_font.h"
#include "ext/video/ag_surfacemanager.h"
#include "ext/video/ag_gltexture.h"
#include "ext/video/ag_surface.h"
#include "ext/video/ag_texture.h"
#include "ext/video/ag_glscreen.h"
#include "ext/video/ag_sdlscreen.h"
#include "ext/video/ag_sgeexport.h"
#include "ext/video/ag_draw.h"
#include "ext/video/ag_png.h"
#include "ext/video/ag_sdlpainter.h"
#include "ext/video/ag_glpainter.h"
#include "ext/video/ag_fbo.h"
#include "ext/video/ag_vdebug.h"
#include "ext/video/ag_rendercontext.h"
#include "ext/video/ag_sdlsurface.h"
#include "ext/video/ag_texturecache.h"
#include "ext/video/ag_clip_painttarget.h"
#include "ext/video/ag_clip.h"
#include "ext/video/ag_projection.h"
#include "ext/video/ag_painter.h"
#include "ext/gui/ag_colorbutton.h"
#include "ext/gui/ag_checkbox.h"
#include "ext/gui/ag_application.h"
#include "ext/gui/ag_widget.h"
#include "ext/gui/ag_tooltip.h"
#include "ext/gui/ag_radio.h"
#include "ext/gui/ag_layout.h"
#include "ext/gui/ag_menu.h"
#include "ext/gui/ag_table.h"
#include "ext/gui/ag_theme.h"
#include "ext/gui/ag_edit.h"
#include "ext/gui/ag_background.h"
#include "ext/gui/ag_listbox.h"
#include "ext/gui/ag_screenwidget.h"
#include "ext/gui/ag_combo.h"
#include "ext/gui/ag_image.h"
#include "ext/gui/ag_frame.h"
#include "ext/gui/ag_button.h"
#include "ext/gui/ag_border.h"
#include "ext/gui/ag_window.h"
#include "ext/gui/ag_layoutfactory.h"
#include "ext/gui/ag_local.h"
#include "ext/gui/ag_text.h"
#include "ext/gui/ag_menuitem.h"
#include "ext/gui/ag_scroller.h"
#include "ext/gui/ag_caption.h"
#include "ext/gui/ag_radiogroup.h"
#include "ext/3dengine/scene_base.h"
#include "ext/3dengine/scene.h"
#include "ext/3dengine/scenenode.h"
#include "ext/3dengine/quadtree.h"
#include "ext/3dengine/ant_camera.h"
#include "ext/3dengine/mesh_sort.h"
#include "ext/3dengine/ant_renderer.h"
#include "ext/3dengine/mesh_data.h"
#include "ext/3dengine/mesh.h"
#include "ext/3dengine/vertex_array.h"
#include "ext/3dengine/glsl.h"
#include "ext/3dengine/mesh_optimizer.h"
#include "ext/3dengine/anim_mesh.h"
#include "ext/3dengine/anim_mesh_data.h"
#include "ext/3dengine/mesh_2d.h"
#include "ext/3dengine/scene_2d.h"
#include "ext/3dengine/mesh_2d_data.h"
#include "ext/3dengine/ant_projection.h"
#include "ext/3dengine/ag_glsl.h"
#include "/home/david/projects/antargis/rant/ext/game/terrain.h"
#include "/home/david/projects/antargis/rant/ext/game/entity.h"
#include "/home/david/projects/antargis/rant/ext/game/height_map.h"
#include "/home/david/projects/antargis/rant/ext/game/ant_app.h"
#include "/home/david/projects/antargis/rant/ext/game/minimap.h"
#include "/home/david/projects/antargis/rant/ext/game/map.h"
#ifdef SWIG
%include "/home/david/projects/antargis/rant/ext/game/terrain.h"
%include "/home/david/projects/antargis/rant/ext/game/entity.h"
%include "/home/david/projects/antargis/rant/ext/game/height_map.h"
%include "/home/david/projects/antargis/rant/ext/game/ant_app.h"
%include "/home/david/projects/antargis/rant/ext/game/minimap.h"
%include "/home/david/projects/antargis/rant/ext/game/map.h"
#endif
#endif
