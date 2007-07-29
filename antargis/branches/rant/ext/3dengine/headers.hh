#ifndef __ANTARGIS_H__
#define __ANTARGIS_H__
#include "ext/basic/ag_config.h"
#include "ext/basic/ag_fs.h"
#include "ext/basic/ag_geometry.h"
#include "ext/basic/ag_main.h"
#include "ext/basic/ag_messageobject.h"
#include "ext/basic/ag_rand_base.h"
#include "ext/basic/ag_rubyobj.h"
#include "ext/basic/ag_serial.h"
#include "ext/basic/ag_singleton.h"
#include "ext/basic/ag_stringstream.h"
#include "ext/basic/ag_string_utf8.h"
#include "ext/basic/ag_utf8.h"
#include "ext/basic/ag_video_base.h"
#include "ext/basic/ag_xml.h"
#include "ext/math/ag_algebra.h"
#include "ext/math/ag_rand.h"
#include "ext/math/ant_frustum.h"
#include "ext/video/ag_clip.h"
#include "ext/video/ag_color.h"
#include "ext/video/ag_font.h"
#include "ext/video/ag_fontengine.h"
#include "ext/video/ag_painter.h"
#include "ext/video/ag_painttarget.h"
#include "ext/video/ag_rendercontext.h"
#include "ext/video/ag_screen.h"
#include "ext/video/ag_surface.h"
#include "ext/video/ag_texture.h"
#include "ext/video/ag_texturecache.h"
#include "ext/video/ag_video.h"
#include "ext/3dengine/mesh_data.h"
#include "ext/3dengine/scene_base.h"
#include "ext/3dengine/scenenode.h"
#include "ext/3dengine/mesh_2d_data.h"
#include "ext/3dengine/anim_mesh_data.h"
#include "ext/3dengine/mesh.h"
#include "ext/3dengine/mesh_2d.h"
#include "ext/3dengine/ant_particle.h"
#include "ext/3dengine/scene.h"
#include "ext/3dengine/anim_mesh.h"
#include "ext/3dengine/ant_camera.h"
#include "ext/3dengine/mesh_optimizer.h"
#include "ext/3dengine/ant_projection.h"
#ifdef SWIG
%include "ext/3dengine/mesh_data.h"
%include "ext/3dengine/scene_base.h"
%include "ext/3dengine/scenenode.h"
%include "ext/3dengine/mesh_2d_data.h"
%include "ext/3dengine/anim_mesh_data.h"
%include "ext/3dengine/mesh.h"
%include "ext/3dengine/mesh_2d.h"
%include "ext/3dengine/ant_particle.h"
%include "ext/3dengine/scene.h"
%include "ext/3dengine/anim_mesh.h"
%include "ext/3dengine/ant_camera.h"
%include "ext/3dengine/mesh_optimizer.h"
%include "ext/3dengine/ant_projection.h"
#endif
#endif
