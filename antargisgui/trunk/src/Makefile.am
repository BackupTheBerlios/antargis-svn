lib_LTLIBRARIES = libantargisgui.la

# WARNING:it is essential that ag_kill.cc stands before ag_widget, because there's a global set in ag_widget, which shouldn't be killed before the singletons!!!
#         additional all references to ag_layoutcreators must be before ag_kill!!

export_h = rubyobj.h ag_messageobject.h \
 ag_geometry.h ag_font.h ag_color.h \
 ag_widget.h ag_colorbutton.h ag_glwidget.h ag_xml.h ag_layout.h ag_dialog.h ag_mutex.h \
 ag_application.h ag_background.h ag_border.h ag_button.h ag_text.h ag_caption.h ag_checkbox.h ag_combo.h ag_edit.h  ag_fontengine.h ag_fs.h ag_painttarget.h ag_surface.h ag_image.h ag_layoutfactory.h \
 ag_vtiler.h \
 ag_listbox.h ag_list.h ag_main.h ag_menu.h ag_menuitem.h ag_mixer.h ag_mutex.h ag_painter.h ag_png.h ag_radio.h ag_screen.h ag_table.h \
 ag_texturecache.h \
 ag_texturemanager.h \
 ag_theme.h \
 ag_tools.h \
 ag_triangle.h \
 ag_window.h \
 ag_xml.h







realsrc_h = $(export_h) GLee.h ag_draw.h ag_config.h ag_exception.h ag_glsurface.h ag_kill.h ag_gettext.h ag_regex.h ag_sdlsurface.h 

realsrc_cc = ag_colorbutton.cc ag_glwidget.cc ag_window.cc ag_surface.cc ag_main.cc privates.cc ag_radio.cc ag_mixer.cc  ag_layout.cc ag_widget.cc ag_geometry.cc ag_font.cc ag_fontengine.cc ag_color.cc ag_messageobject.cc ag_button.cc ag_gradient.cc ag_application.cc ag_text.cc ag_vtiler.cc ag_theme.cc ag_menu.cc ag_menuitem.cc ag_image.cc ag_screen.cc ag_edit.cc ag_debug.cc ag_checkbox.cc ag_table.cc ag_background.cc ag_caption.cc export.c ag_glsurface.cc ag_vector.cc ag_xml.cc ag_tools.cc ag_layoutfactory.cc ag_fs.cc ag_painter.cc ag_sdlsurface.cc ag_border.cc ag_listbox.cc ag_kill.cc ag_triangle.cc ag_png.cc ag_texturecache.cc ag_dialog.cc ag_mutex.cc GLee.c rubyobj.cc



libantargisgui_la_SOURCES = $(realsrc_h) $(realsrc_cc) antargisswig.cc antargisswig.h
libdir = $(RUBY_INSTALL_DIR)
INCLUDES = -I$(RUBY_INCLUDE_DIR)

nantmarker.hh: $(export_h) createmarker.rb
	./createmarker.rb $(export_h)


antargisswig.cc: interface.i $(realsrc_h) nantmarker.hh
	$(SWIGCOMPILE) -o antargisswig.cc interface.i


SWIG = swig
SWIGCOMPILE = $(SWIG) -v -Wall -ruby -c++ -I$(top_srcdir)/include -I$(srcdir) $(INCLUDES)

# includes
#INCLUDES = *.h
EXTRA_DIST = *.h
myincsdir = $(prefix)/include/antargisgui
myincs_HEADERS = *.h
