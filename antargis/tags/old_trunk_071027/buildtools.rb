#!/usr/bin/env ruby

#################################
# buildtools.rb contains
# helper functions for building
# and everything that has
# sth to do with it
#################################



# returns if we're compiling for windows
def windows
	#$mwindows||=nil
	return false if $xcompile
	if $mwindows==nil
		$mwindows||=(have_library("opengl32"))
		if not $mwindows
			$mwindows=false
			puts "no opengl32 found - so expecting _not_ to use windows"
		end
	end
	return $mwindows
end


# test if library x does exist
def test_library(x)
	$failed_libs||=[]
	if not have_library(x)
		$failed_libs.push(x)
	end
end

# test, if there's a program named p - if not output errormessage er
def test_program(p,er)
	path=ENV["PATH"]
	mp=""
	path.split(":").each{|s|
		c=s+"/"+p
		if File.exists?(c)
			mp=c
		end
	}
	if mp==""
		puts "NOTE: "+er
	else
		puts "Found #{p} at #{mp}"
	end
	return mp
end

def bigendian
	[512].pack("f")==[512].pack("g")
end

# profiling doesn't work on PPC for now
if bigendian
	$profile=false
end

def msh(command,&block)
	puts command

  res = system(command)
	status=$?
	if not block.nil?
  	block.call(res, $?)
	end
	if not res
		fail "Command failed with status (#{status.exitstatus}): [#{command}]"
	end
end

windows # check once

if File.exists?("config.rb")
	puts "Config cached! - remove 'config.rb' or 'rake allclean' to clear cache"
	puts
	require "config.rb"
else
  msh "./configure"
	require "config.rb"
end

begin File.mkpath(".deps")
rescue 
end

names={"CC"=>"gcc","CXX"=>"g++","AR"=>"ar","RANLIB"=>"ranlib","NM"=>"nm"}
names.each_key{|n|
	v=nil
	if CONFIG[n]
		v=CONFIG[n]
	else
		v=names[n]
	end
	eval("$"+n+"='"+v+"'")
}
		
def addCleaner(f)
	$files2clean||=[]
	if f.is_a?(String)
		$files2clean.push(f)
	else
		$files2clean+=f
	end
end

if $xcompile
	$CXX="i586-mingw32msvc-g++"
	$CC="i586-mingw32msvc-gcc"
	$AR="i586-mingw32msvc-ar"
	$RANLIB="i586-mingw32msvc-ranlib"
	$NM="i586-mingw32msvc-nm"
end

if $ccache
	$CXX=$ccache+" "+$CXX
	$CC=$ccache+" "+$CC
end

CC=$CC
CONFIG['CC']=$CC
