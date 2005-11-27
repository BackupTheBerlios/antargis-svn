SUBDIRS = glee
lib_LTLIBRARIES = libantargis.la


realsrc_h= anim_mesh.h smoke.h jobs.h entity.h map.h mesh.h terrain.h tree.h scene.h water.h vertex_array.h cg.h pbuffer.h antapp.h glsl.h scenenode.h rubyobj.h mesh_data.h anim_mesh_data.h resource.h entptr.h height_map.h renderer.h ant_config.h
realsrc_cc= anim_mesh.cc smoke.cc jobs.cc entity.cc map.cc mesh.cc terrain.cc tree.cc scene.cc water.cc vertex_array.cc cg.cc pbuffer.cc antapp.cc glsl.cc scenenode.cc rubyobj.cc entptr.cc height_map.cc renderer.cc ant_config.cc


libantargis_la_SOURCES = $(realsrc_h) $(realsrc_cc) antargisruby.cc antargisruby.h
libantargis_la_LIBADD = $(top_builddir)/src/glee/libglee.la

EXTRA_DIST = *.png interface.i

nantmarker.hh: $(realsrc_h) createmarker.rb
	./createmarker.rb $(realsrc_h)

antargisruby.cc: interface.i *.h /usr/local/include/antargisgui/*.h marker.i nantmarker.hh
	$(SWIGCOMPILE) -o antargisruby.cc interface.i


SWIG = swig
SWIGCOMPILE = $(SWIG) -v -Wall -ruby -c++ -I$(top_srcdir)/include -I$(srcdir) $(INCLUDES) -I/usr/local/include/antargisgui

install:
	cp .libs/libantargis.so $(RUBY_INSTALL_DIR)

clean:
	rm antargisruby.cc nantmarker.hh