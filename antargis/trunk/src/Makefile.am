lib_LTLIBRARIES = libantargis.la


realsrc_h= rubyobj.h scenenode.h anim_mesh.h smoke.h jobs.h entity.h height_map.h map.h mesh.h terrain.h tree.h scene.h water.h vertex_array.h cg.h antapp.h glsl.h mesh_data.h anim_mesh_data.h resource.h entptr.h renderer.h ant_config.h new_decal.h mesh_optimizer.h ant_serial.h minimap.h
realsrc_cc= anim_mesh.cc smoke.cc jobs.cc entity.cc map.cc mesh.cc terrain.cc tree.cc scene.cc water.cc vertex_array.cc cg.cc antapp.cc glsl.cc scenenode.cc rubyobj.cc entptr.cc height_map.cc renderer.cc ant_config.cc new_decal.cc mesh_optimizer.cc mesh_data.cc ant_serial.cc minimap.cc


libantargis_la_SOURCES = $(realsrc_h) $(realsrc_cc) antargisruby.cc antargisruby.h

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
