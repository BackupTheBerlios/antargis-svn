TESTS = *.rb mapptest

check_PROGRAMS = mapptest

mapptest_SOURCES = apptest.cc

INCLUDES = -I../include -I$(RUBY_INCLUDE_DIR) $(SDL_CFLAGS)

LDADD = -L../src -lantargisgui -lruby1.8

EXTRA_DIST = *.rb *.ttf *.png
