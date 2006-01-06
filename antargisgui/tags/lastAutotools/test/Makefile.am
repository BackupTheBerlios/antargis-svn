#TESTS = *.rb mapptest

#check_PROGRAMS = mapptest
bin_PROGRAMS = mapptest sdl_reschange

sdl_reschange_SOURCES = sdl_reschange.cc

mapptest_SOURCES = apptest.cc

INCLUDES = -I$(top_srcdir)/src -I$(RUBY_INCLUDE_DIR) $(SDL_CFLAGS)
#CXXFLAGS = 

LDADD = -L../src -lantargisgui -lruby1.8

EXTRA_DIST = *.rb *.ttf *.png

#mythemes_DATA = theme.xml
#mythemesdir = $(datadir)/antargisgui

#mypics_DATA = *.png
#mypicsdir = $(datadir)/antargisgui/pics
