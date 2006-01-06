#!/usr/bin/env ruby
require 'mkmf'
require 'gui/Makefile.rb'
require 'Makefile.rb'


def windows
	# check platform with if /linux/ =~ CONFIG["arch"]



	if $windows==nil
		$windows=have_library("opengl32")
		puts "no opengl32 found - so expecting _not_ to use windows"
	end
	return $windows
end

$prefix="/usr/local"


$bindir=$prefix+"/bin"
$datadir=$prefix+"/share"
$includedir=$prefix+"/include"

dir_config("physfs")
$CFLAGS="-O0 -g"

# check for SDL
sdl_config = with_config('sdl-config', 'sdl-config')
if windows
	sdl_config="c:\\msys\\local\\bin\\sdl-config"
	$LOCAL_LIBS += " -Lc:/msys/local/lib"
	$CFLAGS += " -DUSE_RUBY -Ic:/msys/local/include/SDL -Ic:/msys/local/include"
else
	$CFLAGS += ' -DUSE_RUBY ' + `#{sdl_config} --cflags`.chomp
	$LOCAL_LIBS += ' ' + `#{sdl_config} --libs`.chomp
end

$CXX="g++"

$CFLAGS += " -I./gui"

$INCLUDES = "-I#{Config::CONFIG['archdir']} -I/usr/include/SDL"

cfg = Config::MAKEFILE_CONFIG

puts cfg["CC"]
ccache = with_config('ccache', 'ccache')
if ccache
	cfg["CC"]=ccache+" g++"
else
	cfg["CC"]="g++"
end

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
if windows
	have_library('GLU32')
else
	have_library("GLU")
	have_library("GL")
end
have_library('physfs')
have_library('SDL_mixer')
have_library('SDL_image')
have_library('stdc++')
have_library('z')
have_library('png')

# swig
swig = with_config('swig', 'swig')

# clean swig-generated files
$cleanfiles=["swig.cc","swig.o","swig.h","nantmarker.hh"]

system("touch swig.cc")

create_makefile("libantargisgui",".")


GC.start
mf=File.open("Makefile","a")
mf.puts "swig.cc: interface.i nantmarker.hh "+$interfaceHeaders.join(" ")
mf.puts "	#{swig} -v -Wall -ruby -c++ #{$INCLUDES} -o swig.cc interface.i 2>&1"
mf.puts "nantmarker.hh: "+$interfaceHeaders.join(" ")
mf.puts "	./gui/createmarker.rb "+$interfaceHeaders.join(" ")

mf.puts "install: install-so install-rb install-bin install-headers"

mf.puts "install-bin:"
mf.puts "	$(INSTALL_PROG) -D antargisgui-config #{$bindir}/antargisgui-config"

mf.puts "install-headers:"
$allHeaders.each{|h|
	mf.puts "	$(INSTALL_DATA) -D #{h} #{$includedir}/antargisgui/#{h}"
}
mf.puts "	$(INSTALL_DATA) -D createmarker.rb #{$datadir}/antargisgui/createmarker.rb"


mf.puts "extconf.rb: extconf.rb"
mf.puts "	./extconf.rb"

mf.puts "SRCS = "+$srcs.join(" ")
mf.puts "OBJS = "+$srcs.collect{|x|x.gsub(".cc",".o")}.join(" ")

mf.close


