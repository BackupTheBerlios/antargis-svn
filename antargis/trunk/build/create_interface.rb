#!/usr/bin/env ruby
#--
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
#++
# == Short Description
# This ruby-script generates swig-interface (input)-files out of
# C++-header files.
# The header-files to include are marked with INCLUDE_SWIG somewhere in the file.
# Apart from simple including classes that are derived from AGRubyObject get
# a special treatment to be handled within the garbage collection.
#

require 'build/interface_template.rb'
require 'build/base_tools.rb'
require 'find.rb'
require 'pp'

def vputs(*a)
	puts *a if $verbose
end

def iputs(*a)
	print "   "
	puts *a
end

class MyInput
	attr_reader :swigInput, :outputDir
	def initialize
		@outputDir=Dir.pwd # default
		@swigInput=""
	

		ARGV.each{|a|
			case a
				when /^-d=/
					# in- and output dir
					@outputDir=a[3..-1]
				when /^-i=/
					@swigInput=a[3..-1]
				when /^-v$/
					$verbose=true
				when /^-h/, /^--help/
					@help=true
					puts "Arguments:"
					puts " -h, --help     print this help"
					puts " -d=<DIR>       define output-directory"
					puts " -i=<SWIGFILE>  define SWIG-input-file (swig.h) - can be set multiple times"
					puts " -v             verbose output"
			end
		}

		@swigInput=@swigInput.split(":")
	end

	def valid
		@outputDir!="" && @help.nil?
	end

	def interfaceName
		name=@outputDir+Dir.separator+"interface.i"
		name
	end

	def moduleName
		moduleName=makeLibName(@outputDir)
	end
	def headersName
		@outputDir+Dir.separator+"/headers.hh"
	end
	def markerName
		@outputDir+Dir.separator+"marker.i"
	end
end

def getFiles(dir)
	pattern=dir+"/"+"*.h"
	files=Dir[pattern].select{|f|not f=~/swig.h/} #-[dir+Dir.separator+"swig.h"]
	files  
end

## check if the given string is contained in the file specified by filename
def grepBool(filename,string)
	File.open(filename).each_line{|l|return true if l=~/#{string}/}
	false
end

## select those files that are set be exported through swig
def getSwigInterfaceFiles(files)
	files.select{|f|
		grepBool(f,"INCLUDE_SWIG")
	}
end

class ParsedClasses

	attr_reader :deriveList

	def initialize(files,allfiles)
		@rubyClasses=[]
		@files=files.collect{|f|f.gsub(/.*\/ext\//,"ext/")}
		loadAllDerivations(allfiles.collect{|f|f.gsub(/.*\/ext\//,"ext/")})
		@myfiles=@files #files.collect{|f|f.gsub(/.*ext/,"ext")}
		processDerivations
		initLevels
	end


	def loadAllDerivations(allfiles)
		@class2File={}
		@deriveList={} # x=>y :x is child of y
		@allClasses=[]

		allfiles.each{|fn|
			file=File.open(fn)
			cn=""
			content=file.read.gsub(/\/\*([^*]|\*[^\/])*\*\//,"") # /*...*/ delete comments - FIXME: // comments will be ignored!!!

			content.split("\n").each{|a|
				abak=a
				a.gsub!("AGEXPORT","")
				a.gsub!("EXPORT","")
				
				if a =~ /^ *class.*/ then
					cn=a.gsub(/ *class /,"").gsub(/:.*/,"").gsub("\n","").gsub(" ","")
					@allClasses << cn.gsub(";","")
					if cn=~/^[A-Z].*/
						if a=~ /.*public.*/ then
							pn=a.gsub(/.*public /,"").gsub("\n","")
							@allClasses << pn
							@deriveList[cn]=pn
							@class2File[cn]=fn
						elsif not a=~/;/ then
							@deriveList[cn]=nil
							@class2File[cn]=fn
						end
					end
				end
			
			}
		}
		@classList=@class2File.keys
		@allClasses=@allClasses.sort.uniq
	end

	# check which classes are derived from AGRubyObject and thus handled specifically
	def processDerivations
		@rubyClasses << "AGRubyObject"
		@classList.sort!.uniq!
		
		# check for children of AGWidget
		changed=true
		while changed do
			changed=false
			@deriveList.each {|x,y|
				if @rubyClasses.member?(y) and not @rubyClasses.member?(x) then
					@rubyClasses << x
					changed=true
				end
			}
		end
	end

	## through derivations
	def spreadLevels
		changed=true
		l=@levels.values.max
		while changed
			changed=false
			@deriveList.each{|x,y|
				if @levels[y]==l and @levels[x].nil?
					@levels[x]=l+1
					changed=true
				end
			}
			l+=1
		end
	end

	def initLevels
		vputs "initLevels..."
		@levels={}
		@levels["AGRubyObject"]=0
		spreadLevels

		# give classes with no extisting parent next level
		@classList.each{|c|
			if @levels[c].nil?
				if (not @deriveList.keys.member?(c)) 
					@levels[c]=l
				end
			end
		}

		spreadLevels

		if true
			l=@levels.values.max+1
			# give classes with no parent next level
			@classList.each{|c|
				if @levels[c].nil?
					if (@deriveList[@deriveList[c]].nil?)
						@levels[c]=l
					end
				end
			}
	
			spreadLevels
		end

		l=@levels.values.max
	end

	# in correct order
	def getFileList

		# build file list out of class-order (files may appear several times)
		files=[]
		l=@levels.values.max
		(0..l).each{|i|
            vputs "LEVEL #{i}"
			@levels.each{|n,level|
				if level==i and @class2File[n]
                    vputs n
					files << @class2File[n]
				end
			}
            vputs "----"
		}

		# add files of classes with unknown level
		@classList.each{|c|
			if @levels[c].nil? and @class2File[c]
				files << @class2File[c]
			end
		}
        vputs "myfiles:",@myfiles,"---"
        
        vputs "FILES:",files,"---"
		files=files.select{|f|@myfiles.member?(f)} # select only "my" files - those included in this directory
        vputs "FILES after select:",files,"---"
		addfiles=@files-files
		files+=addfiles                            # add files that are in other directories
        vputs "FILES (add:",files,"---"

		# unique the array
		if files.length>0
			files.uniq!
		end
        vputs "FILES (uniq):",files,"---"

		files
	end


	def getMyRubyClasses
		@rubyClasses.select{|c|@myfiles.member?(@class2File[c])}
	end
	def getAllRubyClasses
		@rubyClasses
	end
	def getAllClasses
		@allClasses
	end
end


def generateInterfaceFile(myInput,files,addfiles)
	filename=myInput.interfaceName
	interfaceI=File.open(filename,"w")
	#puts filename
    #raise 1
	interfaceI.puts interface_template(myInput.moduleName,files,myInput.swigInput,addfiles,myInput.outputDir)
	
	interfaceI.close
	
	filename=myInput.headersName
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
end

def findFilesWith(str)
    files=[]
    Find.find("ext") {|file|files << file}
    files=files.select{|f|f=~/\.h$/}.select{|f|not f=~/swig.h/}.select{|f|File.open(f).read=~/#{str}/}
    #dirs=findDirsRecursively(".")
    #puts "findFilesWith #{str}:"
    #puts files
    #puts "---"
    #exit
    files
    #Dir["*/*"].collect{|f|f.gsub(/\/.*/,"")}.uniq
end

iputs "processing input parameter..."
myInput=MyInput.new

iputs "getting input interface files (of current directory #{myInput.outputDir})..."
files=getSwigInterfaceFiles(getFiles(myInput.outputDir))
vputs "--"
vputs files.join("\n")
vputs "--"
iputs "found #{files.length} files"
iputs "filtering INCLUDE_SWIG to get header files (all of the whole project)..."
cfiles=findFilesWith("INCLUDE_SWIG")
iputs "found #{cfiles.length} files"
vputs "--"
vputs cfiles.join("\n")
vputs "--"
#exit

#parsedClasses=ParsedClasses.new(files,`find $(pwd) -name "*.h"|grep -v swig`.split("\n"))
parsedClasses=ParsedClasses.new(files,cfiles)
files=parsedClasses.getFileList

addfiles=[]
myInput.swigInput.each{|inDir|
  vputs "inDir #{inDir}"
	pattern=getDirUnix(inDir)+"/*.h"
	vputs "PATTERN:",pattern,"!!!!"
	addfiles+=Dir[pattern].select{|f|not f=~/swig.h/}.select{|f|File.open(f).read=~/INCLUDE_SWIG/}
}

generateInterfaceFile(myInput,files,addfiles)

file=File.open(myInput.markerName,"w")

# ok, first marking is included
parsedClasses.getMyRubyClasses.each {|x|
	#if y then
		file.puts "%exception "+x+"::"+x+" {"
		file.puts "	$action"
		file.puts "	result->mRUBY=self;"
		file.puts "#ifdef GCDEBUG"
		file.puts '     result->mObjName=typeid(*result).name();'
		#file.puts '     printf("%lx   %s\n",self,typeid(*result).name());'
		file.puts "#endif"
		file.puts "	result->mRubyObject=true;"
		file.puts "}"
		file.puts "%markfunc "+x+" \"general_markfunc\""
	#end
}

# swig typemaps
# so that always the lowest children in a derivation hierarchy is returned
myRubyClasses=parsedClasses.getMyRubyClasses
parsedClasses.deriveList.keys.each{|s|
	if myRubyClasses.member?(s)
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

classList=parsedClasses.getAllClasses
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

rubyClasses=parsedClasses.getMyRubyClasses
allRubyClasses=parsedClasses.getAllRubyClasses

myClasses=parsedClasses.getMyRubyClasses

file.puts <<EOT
%{

#undef write
#undef read

// cast-function map
// it contains the mapping from parent-classes=>dyn-cast-functions to child-classes
#include <string>
#include <map>
#include <list>
#include <iostream>

typedef swig_type_info*(*CastFunction)(void**);
extern std::map<std::string,std::list<CastFunction> > agCastFunctions;
%}
EOT
pp myClasses
myClasses.each{|k|
file.puts <<EOT	
%{
// try to cast from #{k} to one of its child classes (if they exist)
static swig_type_info* #{k}_dynamic_cast(void **ptr)
  {
	  std::list<CastFunction> &funcs=agCastFunctions["#{k}"];
    for(std::list<CastFunction>::iterator i=funcs.begin();i!=funcs.end();i++)
      {
         swig_type_info*t=(*i)(ptr);
	       //std::cout<<"test:"<<t<<std::endl;
         if(t)
           return t;
      }
   return 0;
 }
%}
DYNAMIC_CAST(SWIGTYPE_p_#{k}, #{k}_dynamic_cast);
EOT
}

parsedClasses.deriveList.each{|b,a|
	if rubyClasses.member?(b) and allRubyClasses.member?(a)
		# for each pair generate a casting function and register it into agCastFunctions
file.puts <<EOT
%{
swig_type_info* #{a}2#{b}cast(void **p)
{
        #{a}*a=(#{a}*)(*p);
        #{b}*b=dynamic_cast<#{b}*>(a);
				//printf("TRY CAST #{a} 2 #{b} : %lx\\n",b);
        if(b)
        {
                *p=(void*)b;
                return SWIGTYPE_p_#{b};
        }
        return 0;
}
%}

%insert("init") %{
        agCastFunctions["#{a}"].push_back(&#{a}2#{b}cast);
%}


EOT
	end
}



