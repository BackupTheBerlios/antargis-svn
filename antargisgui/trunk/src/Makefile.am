lib_LTLIBRARIES = libantargisgui.la

# WARNING:it is essential that ag_kill.cc stands before ag_widget, because there's a global set in ag_widget, which shouldn't be killed before the singletons!!!
#         additional all references to ag_layoutcreators must be before ag_kill!!

libantargisgui_la_SOURCES = ag_colobutton.h ag_colorbutton.cc ag_glwidget.cc ag_glwidget.h ag_window.cc ag_surface.cc ag_main.cc privates.cc  ag_radio.cc ag_mixer.cc  ag_layout.cc ag_widget.cc ag_geometry.cc ag_font.cc ag_fontengine.cc ag_color.cc ag_messageobject.cc ag_button.cc ag_gradient.cc ag_application.cc ag_text.cc ag_vtiler.cc ag_theme.cc ag_menu.cc ag_menuitem.cc ag_image.cc ag_screen.cc ag_edit.cc ag_debug.cc ag_checkbox.cc ag_table.cc ag_background.cc ag_caption.cc export.c ag_glsurface.cc ag_vector.cc ag_xml.h ag_xml.cc ag_tools.cc ag_layoutfactory.cc ag_fs.cc ag_painter.cc ag_sdlsurface.cc ag_border.cc ag_listbox.cc ag_kill.cc ag_triangle.cc antargisswig.cc ag_png.cc ag_texturecache.cc
libdir = $(RUBY_INSTALL_DIR)
INCLUDES = -I$(RUBY_INCLUDE_DIR)


marker.h: mk_marker_h.rb ag_*.h
	./mk_marker_h.rb >marker.h

antargisswig.cc: interface.i *.h marker.h
	$(SWIGCOMPILE) -o antargisswig.cc interface.i


SWIG = swig
SWIGCOMPILE = $(SWIG) -v -Wall -ruby -c++ -I$(top_srcdir)/include -I$(srcdir) $(INCLUDES)

# includes
#INCLUDES = *.h
EXTRA_DIST = *.h
myincsdir = $(prefix)/include/antargisgui
myincs_HEADERS = *.h
