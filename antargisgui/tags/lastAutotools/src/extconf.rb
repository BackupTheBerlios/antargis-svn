#!/usr/bin/env ruby
require 'mkmf'


dir_config("physfs")

# check for SDL
sdl_config = with_config('sdl-config', 'sdl-config')
sdl_config="c:\\msys\\local\\bin\\sdl-config"
#$CFLAGS += ' ' + `#{sdl_config} --cflags`.chomp
#$LOCAL_LIBS += ' ' + `#{sdl_config} --libs`.chomp

$CFLAGS="-O0 -g"

$CFLAGS += " -DUSE_RUBY -Ic:/msys/local/include/SDL -Ic:/msys/local/include"
$LOCAL_LIBS += " -Lc:/msys/local/lib"
$CXX="g++"

    cfg = Config::MAKEFILE_CONFIG

		#puts cfg
		puts cfg["CC"]
		cfg["CC"]="g++"

if not have_header('SDL.h')
	puts "Please install libSDL"
	exit
end

if not have_header('physfs.h')
	puts "Please install libSDL"
	exit
end

have_library('SDL')
have_library('SDL_ttf')
have_library('SGE')
have_library("opengl32")
have_library('GLU32')
have_library('physfs')
have_library('SDL_mixer')
have_library('SDL_image')
have_library('stdc++')
have_library('z')
have_library('png')

# swig
swig = with_config('swig', 'swig')
if swig
	require 'mk_marker_h.rb'
	#`#{swig} -v -Wall -ruby -c++  -o antargisswig.cc interface.i`
	`#{swig} -ruby -c++  -o antargisswig.cc interface.i 2>&1|grep -v Warning`
end


create_makefile("libantargisgui")
