#TESTS = *.rb mapptest

#check_PROGRAMS = mapptest
bin_PROGRAMS = mapptest

mapptest_SOURCES = apptest.cc

INCLUDES = -I$(top_srcdir)/include -I$(RUBY_INCLUDE_DIR) $(SDL_CFLAGS)
#CXXFLAGS = 

LDADD = -L../src -lantargisgui -lruby1.8

EXTRA_DIST = *.rb *.ttf *.png

mythemes_DATA = theme.xml
mythemesdir = $(datadir)/antargisgui

mypics_DATA = *.png
mypicsdir = $(datadir)/antargisgui/pics
