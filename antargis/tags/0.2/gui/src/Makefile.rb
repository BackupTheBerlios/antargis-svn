#!/usr/bin/env ruby

$interfaceHeaders||=[]
$interfaceHeaders+=["ag_rubyobj.h","ag_messageobject.h",
 "ag_geometry.h","ag_font.h","ag_color.h","ag_local.h","ag_utf8.h",
 "ag_widget.h","ag_colorbutton.h","ag_glwidget.h","ag_xml.h","ag_layout.h","ag_dialog.h","ag_mutex.h",
 "ag_application.h","ag_background.h","ag_border.h","ag_button.h","ag_text.h","ag_caption.h","ag_checkbox.h","ag_combo.h","ag_edit.h","ag_fontengine.h","ag_fs.h","ag_painttarget.h","ag_texture.h","ag_surface.h","ag_gltexture.h","ag_surfacemanager.h","ag_image.h","ag_layoutfactory.h","ag_listbox.h","ag_list.h","ag_main.h","ag_menu.h","ag_menuitem.h","ag_mixer.h","ag_mutex.h","ag_painter.h","ag_png.h","ag_radio.h","ag_screen.h","ag_glsurface.h","ag_table.h","ag_texturecache.h","ag_theme.h","ag_tools.h","ag_window.h","ag_xml.h","ag_frame.h","ag_screenwidget.h","ag_glsl.h","ag_scroller.h","ag_plugin.h","ag_aes.h","ag_destructor.h"]

#$interfaceHeader+=["ag_edit2.h"]

$addHeaders||=[]
$addHeaders+=["GLee.h","ag_draw.h","ag_config.h","ag_exception.h","ag_glsurface.h","ag_kill.h","ag_gettext.h","ag_regex.h","ag_sdlsurface.h","ag_debug.h","ag_geometry_inline.cc"]

$allHeaders=$interfaceHeaders + $addHeaders

#puts $interfaceHeaders
	
#$interfaceHeaders+=['slimrqs.h','calg.h','mycrypt.h']
