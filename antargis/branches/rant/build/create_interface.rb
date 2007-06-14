#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# create_interface.rb
# by David Kamphausen (david.kamphausen@web.de)
#
# The "Antargis" project, including all files needed to compile it,
# is free software; you can redistribute it and/or use it and/or modify it
# under the terms of the GNU General Public License as published
# by the Free Software Foundation; either version 2 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# You should have received a copy of the GNU General Public
# License along with this program.
#

require 'build/interface_template.rb'
require 'build/base_tools.rb'

outputDir=Dir.pwd # default
swigInput=""


ARGV.each{|a|
	case a
		when /^-d=/
			# in- and output dir
			outputDir=a[3..-1]
		when /^-i=/
			swigInput=a[3..-1]
		when /^-h/, /^--help/
			puts "Arguments:"
			puts " -h, --help     print this help"
			puts " -d=<DIR>       define output-directory"
			puts " -i=<SWIGFILE>  define SWIG-input-file (swig.h) - can be set multiple times"
	end
}

swigInput=swigInput.split(":")

puts "DIR:"
puts outputDir
#exit
class MClass
  def initialize(name,superClass)
    @name=name
    @superClass=superClass
  end
end

def getDirs
  files=`find ext -type d`.gsub("\r","").split("\n")
end

def getFiles(dir)
	Dir[dir+Dir.separator+"*.h"].select{|f|not f=~/swig.h/} #-[dir+Dir.separator+"swig.h"]
end

def grepBool(filename,string)
	File.open(filename).each_line{|l|return true if l=~/#{string}/}
	false
end

def getSwigInterfaceFiles(files)
	files.select{|f|
		grepBool(f,"INCLUDE_SWIG")
	}
end

def getClasses
end


files=getSwigInterfaceFiles(getFiles(outputDir))

addfiles=[]
swigInput.each{|inDir|
	pattern=getDir(inDir)+Dir.separator+"*.h"
	puts "PATTERN:",pattern
	addfiles+=Dir[pattern].select{|f|not f=~/swig.h/}
}


filename=outputDir+Dir.separator+"/interface.i"
puts "filename:",filename
interfaceI=File.open(filename,"w")

moduleName="Libantargis"
moduleName=makeLibName(outputDir)
interfaceI.puts interface_template(moduleName,files,swigInput,addfiles)

interfaceI.close

filename=outputDir+Dir.separator+"/headers.hh"
headersH=File.open(filename,"w")
headersH.puts "#ifndef __ANTARGIS_H__"
headersH.puts "#define __ANTARGIS_H__"
headersH.puts addfiles.collect{|f|"#include \"#{f}\""}.join("\n")
headersH.puts files.collect{|f|"#include \"#{f}\""}.join("\n")
headersH.puts "#ifdef SWIG"
headersH.puts files.collect{|f|"%include \"#{f}\""}.join("\n")
headersH.puts "#endif"
headersH.puts "#endif"
headersH.close



puts "GETFILES:"

puts files.join("|")






#exit

# old implementation


classList=[]
deriveList={} # x=>y :x is child of y
rubyClasses={} # all, which are derived from AGRubyObject
class2File={}




#files=ARGV

# take RubyObject as base for RubyObjects :-)
# so simply search for classes deriveListd from this!

files.each{|fn|
	g=File.open(fn)
	cn=""
	g.each{|a|
		a.gsub!("AGEXPORT","")
		a.gsub!("EXPORT","")
		
		if a =~ /^class.*/ then
			cn=a.gsub("class ","").gsub(/[:;].*/,"").gsub(/\n/,"").gsub(" ","")
			classList+=[cn]
			if a=~ /.*public.*/ then
				pn=a.gsub(/.*public /,"").gsub(/\n/,"")
				deriveList[cn]=pn
				rubyClasses[cn]=false
				rubyClasses[pn]=false
				class2File[cn]=fn
			end
		end
	
	}
}

rubyClasses["AGRubyObject"]=true

classList.sort!.uniq!

# check for children of AGWidget
changed=true
while changed do
	changed=false
	deriveList.each {|x,y|
		if rubyClasses[y] and rubyClasses[x]==false then
			rubyClasses[x]=true
			changed=true
		end
	}
end

file=File.open(outputDir+Dir.separator+"marker.i","w")

# ok, first marking is included
rubyClasses.each {|x,y|
	if y then
		file.puts "%exception "+x+"::"+x+" {"
		file.puts "	$action"
		file.puts "	result->mRUBY=self;"
		file.puts "#ifdef GCDEBUG"
		file.puts '     result->mObjName=typeid(*result).name();'
		file.puts '     printf("%lx   %s\n",self,typeid(*result).name());'
		file.puts "#endif"
		file.puts "	result->mRubyObject=true;"
		file.puts "}"
		file.puts "%markfunc "+x+" \"general_markfunc\""
	end
}

# calculate class-derivations
derivations={}
rubyClasses.each{|x,y|
	derivations[x]=[]
}
deriveList.each{|x,y|
	if not derivations[y]
		derivations[y]=[]
	end
	derivations[y].push(x)
}
changed=true
while changed
	changed=false
	derivations.each{|x,y|
		y.each{|a|
			if derivations[a]
				old=derivations[x]
				derivations[x]+=derivations[a]
				derivations[x].sort!
				derivations[x].uniq!
				#puts x+":"+old.length.to_s+" "+derivations[x].length.to_s
				if old.length<derivations[x].length
					changed=true
				end
			end
		}
	}
end

# swig typemaps
# so that always the lowest children in a derivation hierarchy is returned

deriveList.keys.each{|s|
	if rubyClasses[s]
		file.puts "%typemap(out) #{s}*{"
		file.puts " if($1)"
		file.puts " {"
		file.puts "   $result=AG_NewPointerObj($1,SWIGTYPE_p_#{s},0);"
		file.puts " }"
		file.puts "}"

		file.puts "%typemap(directorin) #{s}*{"
		file.puts " if($1)"
		file.puts " {"
		file.puts "   $input=AG_NewPointerObj($1,SWIGTYPE_p_#{s},0);"
		file.puts " }"
		file.puts "}"

	end
}
# normal typemaps
classList.each{|c|
	file.puts "%typemap(directorout) #{c} {"
	file.puts " #{c} *b;"
	file.puts " Data_Get_Struct($input,#{c},b);"
	file.puts " $result=*b;"
	file.puts "}"
}

file.puts "%typemap(directorout) Uint8 {"
file.puts " $result=NUM2INT($input);"
file.puts "}"


truncClasses={}

deriveList.each{|b,a|
	if rubyClasses[b] and rubyClasses[a]
		#puts "DERIVE #{a} #{b}"
		truncClasses[a]||=[]
		truncClasses[a] << b
	end
}
truncClasses.each{|k,a|
	file.puts <<EOT
%{
static swig_type_info* #{k}_dynamic_cast(void **ptr) {
EOT
			a.each{|x|
				file.puts <<EOT
  {
		#{x} *e = dynamic_cast<#{x} *>((#{k}*)*ptr);
		if (e) 
		{
			*ptr = (void *) e;
			return SWIGTYPE_p_#{x};
		}
  }
EOT
			}
	file.puts <<EOT
  return 0;
 }
%}
DYNAMIC_CAST(SWIGTYPE_p_#{k}, #{k}_dynamic_cast);
EOT


}
exit

file.close

# now generate antargis.h
#filename="antargisgui.h"
filename=outputDir+Dir.separator+"/headers.hh"
file=File.open(filename,"w")

file.puts "#ifndef __ANTARGIS_H__"
file.puts "#define __ANTARGIS_H__"
files.each{|f|
	file.puts "#include \"#{f}\""
}
file.puts "#include <SDL_keysym.h>"
file.puts "#ifdef SWIG"
files.each{|f|
	file.puts "%include \"#{f}\""
}
file.puts '%include "/usr/include/SDL/SDL_keysym.h"'
file.puts "#endif"
file.puts "#endif"


file.close

puts "created nantmarker.hh"
