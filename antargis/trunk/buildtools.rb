#!/usr/bin/env ruby

def windows
	if $mwindows==nil
		$mwindows||=(have_library("opengl32"))
		if not $mwindows
			
			puts "no opengl32 found - so expecting _not_ to use windows"
		end
	end
	#puts "WINDOWS:"+$mwindows.to_s
	return $mwindows
end

$failed_libs=[]
def test_library(x)
	if not have_library(x)
		$failed_libs.push(x)
	end
end
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
	#puts mp	
	#x=`whereis #{p}`
	#puts x,path
	return mp
end

def bigendian
	[512].pack("f")==[512].pack("g")
end

if bigendian
	$profile=false
end
