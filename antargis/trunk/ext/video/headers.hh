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
#include "ext/basic/ag_string_utf8.h"
#include "ext/basic/ag_stringstream.h"
#include "ext/basic/ag_utf8.h"
#include "ext/basic/ag_video_base.h"
#include "ext/basic/ag_xml.h"
#include "ext/sound/ag_mixer.h"
#include "ext/math/ag_algebra.h"
#include "ext/math/ag_rand.h"
#include "ext/math/ant_frustum.h"
#include "ext/video/ag_painttarget.h"
#include "ext/video/ag_surface.h"
#include "ext/video/ag_texture.h"
#include "ext/video/ag_screen.h"
#include "ext/video/ag_fontengine.h"
#include "ext/video/ag_video.h"
#include "ext/video/ag_clip.h"
#include "ext/video/ag_rendercontext.h"
#include "ext/video/ag_texturecache.h"
#include "ext/video/ag_font.h"
#include "ext/video/ag_painter.h"
#include "ext/video/ag_color.h"
#ifdef SWIG
%include "ext/video/ag_painttarget.h"
%include "ext/video/ag_surface.h"
%include "ext/video/ag_texture.h"
%include "ext/video/ag_screen.h"
%include "ext/video/ag_fontengine.h"
%include "ext/video/ag_video.h"
%include "ext/video/ag_clip.h"
%include "ext/video/ag_rendercontext.h"
%include "ext/video/ag_texturecache.h"
%include "ext/video/ag_font.h"
%include "ext/video/ag_painter.h"
%include "ext/video/ag_color.h"
#endif
#endif
