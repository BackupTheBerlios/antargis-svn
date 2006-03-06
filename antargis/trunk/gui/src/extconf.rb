#!/usr/bin/env ruby
require 'mkmf'
require 'Makefile.rb'

def windows
	if $windows==nil
		$windows=have_library("opengl32")
		if not $windows
		  puts "no opengl32 found - so expecting _not_ to use windows"
		end
	end
	return $windows
end

$prefix="/usr/local"


$bindir=$prefix+"/bin"
$datadir=$prefix+"/share"
$includedir=$prefix+"/include"

$CFLAGS="-O0 -g -Wall"

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

$INCLUDES = "-I#{Config::CONFIG['archdir']} -I/usr/include/SDL"

cfg = Config::MAKEFILE_CONFIG

puts cfg["CC"]
if windows
	cfg["CC"]="g++"
else
	ccache = with_config('ccache', 'ccache')
	if ccache
		cfg["CC"]=ccache+" g++"
	else
		cfg["CC"]="g++"
	end
end
if not have_header('SDL.h')
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
have_library('SDL_mixer')
have_library('SDL_image')
have_library('stdc++')
have_library('z')
have_library('png')

create_makefile("libantargisgui")
GC.start
mf=File.open("Makefile","a")

if not windows
	
	# swig
	swig = with_config('swig', 'swig')
	
	# clean swig-generated files
	$cleanfiles=["swig.cc","swig.o","swig.h","nantmarker.hh"]
	
	mf.puts "swig.cc: interface.i nantmarker.hh "+$interfaceHeaders.join(" ")
	mf.puts "	#{swig} -v -Wall -ruby -c++ #{$INCLUDES} -o swig.cc interface.i 2>&1"
	mf.puts "nantmarker.hh: "+$interfaceHeaders.join(" ")
	mf.puts "	./createmarker.rb "+$interfaceHeaders.join(" ")
	
end

mf.puts "install: install-so install-rb install-bin install-headers"

mf.puts "install-bin:"
mf.puts "	$(INSTALL_PROG) -D antargisgui-config #{$bindir}/antargisgui-config"

mf.puts "install-headers:"
$allHeaders.each{|h|
	mf.puts "	$(INSTALL_DATA) -D #{h} #{$includedir}/antargisgui/#{h}"
}
mf.puts "	$(INSTALL_DATA) -D createmarker.rb #{$datadir}/antargisgui/createmarker.rb"


mf.close


# create antargisgui-config

mf=File.open("antargisgui-config","w")
mf.puts File.open("antargisgui-config.in","r").read
mf.puts File.open("Makefile.rb","r").readlines[1..1000].join("") # copy skipping first line
mf.puts "$prefix='#{$prefix}'"
mf.puts "go"
mf.close


system("rm swig.cc")
system("rm nantmarker.hh")
