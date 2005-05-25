lib_LTLIBRARIES = libantargis.la
#bin_PROGRAMS = antargis
#antargis_SOURCES = antargis.cpp 

libantargis_la_SOURCES = ant_geometry.cpp ant_rect.cpp decast.cpp game.cpp \
 map.cpp md5.cpp minimap.cpp mpng.cpp mystring.cpp rain.cpp store.cpp texture_cache.cpp \
 tree.cpp view.cpp voxel.cpp voxel_gen.cpp ant_geometry.h ant_rect.h cvinfo.h decast.h \
 game.h map.h md5.h minimap.h mpng.h myapp.h mystring.h pmd5.h quadtree.h \
 rain.h random_map.h serialize.h store.h texture_cache.h tree.h view.h voxel_gen.h voxel.h antargisruby.cc antargisruby.h \
 jobs.h jobs.cc entity.cc entity.h
LDADD = -lantargis

# old:
#ant_house.h ant_house.cc  ant_hero.h ant_hero.cc  ant_tree.h ant_tree.cc ant_boss.h ant_boss.cc ant_man.h ant_man.cc


marker.h: mk_marker_h.rb /usr/local/include/antargisgui/*.h
	./mk_marker_h.rb >marker.h

antargisruby.cc: interface.i *.h /usr/local/include/antargisgui/*.h
	$(SWIGCOMPILE) -o antargisruby.cc interface.i


SWIG = swig
SWIGCOMPILE = $(SWIG) -v -Wall -ruby -c++ -I$(top_srcdir)/include -I$(srcdir) $(INCLUDES) -I/usr/local/include/antargisgui

EXTRA_DIST = \
	interface.i

clean:
	rm antargisruby.cc antargisruby.h

#libdir = $(top_srcdir)
install:
	cp .libs/libantargis.so $(RUBY_INSTALL_DIR)

#LIBADD = -lantargisgui
libantargis_la_LIBADD = -lantargisgui
