#SUBDIRS = lua
lib_LTLIBRARIES = libantargisruby.la
libantargisruby_la_SOURCES = antargisswig.cc

marker.h: mk_marker_h.rb ../include/*.h
	./mk_marker_h.rb >marker.h

antargisswig.cc: interface.i ../include/*.h *.h marker.h
	$(SWIGCOMPILE) -o antargisswig.cc interface.i


SWIG = swig
SWIGCOMPILE = $(SWIG) -v -Wall -ruby -c++ -I$(top_srcdir)/include -I$(srcdir) $(INCLUDES)

libantargisruby_la_LIBADD = -lantargisgui

INCLUDES = \
	$(SDL_CFLAGS) \
	-I$(top_srcdir)/include \
	-I$(RUBY_INCLUDE_DIR) \
	-I../../include 
#-I/usr/include/c++/3.3 -I/usr/include/c++/3.3/i486-linux -I/usr/include -I/usr/include/linux/

libdir = $(RUBY_INSTALL_DIR)

#pararubydir = $(RUBY_INSTALL_DIR)

#pararuby_DATA = \
#	libantargisruby.so

EXTRA_DIST = \
	interface.i antargisswig.h *.rb *.i

clean:
	rm *.cc marker.h

