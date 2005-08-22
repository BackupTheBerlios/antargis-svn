lib_LTLIBRARIES = libantargis.la
libantargis_la_SOURCES = jobs.cc jobs.h entity.h entity.cc map.h map.cc mesh.h mesh.cc terrain.h terrain.cc tree.h tree.cc scene.h scene.cc water.h water.cc vertex_array.h vertex_array.cc cg.cc cg.h pbuffer.h pbuffer.cc antapp.h antapp.cc antargisruby.cc

EXTRA_DIST = *.png interface.i

antargisruby.cc: interface.i *.h /usr/local/include/antargisgui/*.h marker.i
	$(SWIGCOMPILE) -o antargisruby.cc interface.i


SWIG = swig
SWIGCOMPILE = $(SWIG) -v -Wall -ruby -c++ -I$(top_srcdir)/include -I$(srcdir) $(INCLUDES) -I/usr/local/include/antargisgui

install:
	cp .libs/libantargis.so $(RUBY_INSTALL_DIR)
