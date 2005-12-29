#!/usr/bin/env ruby
require 'mkmf'

# check for SDL
sdl_config = with_config('sdl-config', 'sdl-config')
$CFLAGS += ' ' + `#{sdl_config} --cflags`.chomp
$LOCAL_LIBS += ' ' + `#{sdl_config} --libs`.chomp

$CFLAGS += " -DUSE_RUBY"
$CXX="g++"

    cfg = Config::MAKEFILE_CONFIG

		#puts cfg
		puts cfg["CC"]
		cfg["CC"]="g++"

if not have_header('SDL.h')
	puts "Please install libSDL"
	exit
end

have_library('SDL') #, 'SDL_Init')
have_library('SDL_ttf') #, 'TTF_Init')
have_library('SGE') #, 'sge_FilledRect')
have_library('GLU') #, 'gluPerspective')
have_library('physfs') #, 'PHYSFS_init')
have_library('SDL_mixer') #, 'Mix_OpenAudio')
#have_library('SDL_mixer', 'Mix_OpenAudio')


# swig
swig = with_config('swig', 'swig')
if swig
	require 'mk_marker_h.rb'
	#`#{swig} -v -Wall -ruby -c++  -o antargisswig.cc interface.i`
	`#{swig} -ruby -c++  -o antargisswig.cc interface.i 2>&1|grep -v Warning`
end


create_makefile("libantargisgui")
