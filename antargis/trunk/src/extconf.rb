#!/usr/bin/env ruby
require 'mkmf'
require 'ftools'


$CFLAGS="-O0 -g"

$CFLAGS += " -DUSE_RUBY -Ic:/msys/local/include/SDL -Ic:/msys/local/include -Ic:/msys/home/root/antargisgui/src"
$LOCAL_LIBS += " -Lc:/msys/local/lib -Lc:/msys/home/root/antargisgui/src"
$CXX="g++"

cfg = Config::MAKEFILE_CONFIG

puts cfg["CC"]
cfg["CC"]="g++"

check_hs={'SDL.h'=>"libSDL",'physfs.h'=>"physfs"} #,'ag_triangle.h'=>"antargisgui"}

check_hs.each{|h,n|
	if not have_header(h)
		puts "Please install #{n}"
		exit
	end
}


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
have_library('antargisgui')


srcdirs=[".","c:/msys/home/root/antargisgui/src"]
builddir="build"

files=[]
srcdirs.each{|d|
	Dir[File.join(d,"*.{cc,c}")].each{|f|
		if not (d=~/antargisgui/ and f=~/swig/)
			files.push(f)
		end
	}
}
begin
	Dir.mkdir(builddir)
rescue
end
files.each{|f|
	File.cp(f,File.join(builddir,File.basename(f)))
}

create_makefile("libantargis",builddir)
