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
#include "/home/david/projects/antargis/rant/ext/3dengine/mesh_data.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/scene_base.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/scenenode.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/mesh_2d_data.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/anim_mesh_data.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/mesh.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/mesh_2d.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/ant_particle.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/scene.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/anim_mesh.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/ant_camera.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/mesh_optimizer.h"
#include "/home/david/projects/antargis/rant/ext/3dengine/ant_projection.h"
#ifdef SWIG
%include "/home/david/projects/antargis/rant/ext/3dengine/mesh_data.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/scene_base.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/scenenode.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/mesh_2d_data.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/anim_mesh_data.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/mesh.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/mesh_2d.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/ant_particle.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/scene.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/anim_mesh.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/ant_camera.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/mesh_optimizer.h"
%include "/home/david/projects/antargis/rant/ext/3dengine/ant_projection.h"
#endif
#endif