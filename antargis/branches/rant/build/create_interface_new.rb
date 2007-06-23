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
				when /^-h/, /^--help/
					@help=true
					puts "Arguments:"
					puts " -h, --help     print this help"
					puts " -d=<DIR>       define output-directory"
					puts " -i=<SWIGFILE>  define SWIG-input-file (swig.h) - can be set multiple times"
			end
		}

		@swigInput=@swigInput.split(":")
	end

	def valid
		@outputDir!="" && @help.nil?
	end

	def interfaceName
		name=@outputDir+Dir.separator+"interface.i"
		puts "INTERFACENAME:",name
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

class MClass
  def initialize(name,superClass)
    @name=name
    @superClass=superClass
  end
end

class SimpleParser
	@mclasses=[]
	def initialize(files)
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

class ParsedClasses

	attr_reader :deriveList

	def initialize(files,allfiles)
		@rubyClasses=[]
		@files=files
		loadAllDerivations(allfiles)
		@myfiles=files
		processDerivations
		initLevels
	end


	def loadAllDerivations(allfiles)
		@class2File={}
		@deriveList={} # x=>y :x is child of y

		allfiles.each{|fn|
			g=File.open(fn)
			cn=""
			g.each{|a|
				abak=a
				a.gsub!("AGEXPORT","")
				a.gsub!("EXPORT","")
				
				if a =~ /^class.*/ then
					cn=a.gsub("class ","").gsub(/:.*/,"").gsub("\n","").gsub(" ","")
					if cn=~/^[A-Z].*/
						if a=~ /.*public.*/ then
							pn=a.gsub(/.*public /,"").gsub("\n","")
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
	end

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
		puts "-------------------------"
		puts "CLASSLIST:"
		puts @classList

		puts "-------------------------"
		puts "RUBY CLASSLIST:"
		puts @rubyClasses

		puts "-------------------------"
		puts "DERIVATIONS:"
			@deriveList.each {|x,y|
				puts "#{x} #{y} #{@class2File[x]} "
			}
		puts "-------------------------"
		puts "my ruby:"
		puts getMyRubyClasses
		puts "-------------------------"
	end

	def initLevels
		puts "initLevels..."
		@levels={}
		@levels["AGRubyObject"]=0
		changed=true
		l=0
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

		(0..l).each{|i|
			@levels.each{|n,level|
				if level==i
					puts "#{i} #{n}"
				end
			}
		}
	#if false
		puts "-------------"		
		# give classes with no parent next level
		@classList.each{|c|
			if @levels[c].nil?
				if (not @deriveList.keys.member?(c)) 
					@levels[c]=l
				end
			end
		}

		# repeat process
		changed=true
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

		puts "-------------"		
		# give classes with no parent next level
		@classList.each{|c|
			if @levels[c].nil?
				if (@deriveList[@deriveList[c]].nil?)
					@levels[c]=l
				end
			end
		}

		# repeat process
		changed=true
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

#end
		(0..l).each{|i|
			@levels.each{|n,level|
				if level==i
					puts "#{i} #{n}"
				end
			}
		}
		@classList.each{|c|
			if @levels[c].nil?
				puts "-- #{c}"
			end
		}

		puts "initLevels...ok"

	end

	# in correct order
	def getFileList
		files=[]
		l=@levels.values.max
		(0..l).each{|i|
			@levels.each{|n,level|
				if level==i and @class2File[n]
					files << @class2File[n]
				end
			}
		}

		puts "getFileList:"
		puts files,files.class
		puts "---------"

		@classList.each{|c|
			if @levels[c].nil? and @class2File[c]
				files << @class2File[c]
			end
		}

		puts "getFileList:"
		puts files,files.class
		puts "---------"
		files=files.select{|f|@myfiles.member?(f)}
		addfiles=@files-files
		files+=addfiles


		puts "getFileList:"
		puts files,files.class
		puts "---------"
		if files.length>0
			files.uniq!
		end
		puts files,files.class
		puts "---------"
		files
	end


	def getMyRubyClasses
		@rubyClasses.select{|c|@myfiles.member?(@class2File[c])}
	end
	def getAllRubyClasses
		@rubyClasses
	end
end


def generateInterfaceFile(myInput,files,addfiles)
	filename=myInput.interfaceName
	puts "filename:",filename
	interfaceI=File.open(filename,"w")
	
	
	puts "SWIGINPUTS:",myInput.swigInput,"--------------"
	
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


myInput=MyInput.new

files=getSwigInterfaceFiles(getFiles(myInput.outputDir))

parsedClasses=ParsedClasses.new(files,`find $(pwd) -name "*.h"|grep -v swig`.split("\n"))
files=parsedClasses.getFileList

#exit

puts "GETFILES:",files,files.class

puts files.join("|")

addfiles=[]
myInput.swigInput.each{|inDir|
	pattern=getDir(inDir)+Dir.separator+"*.h"
	puts "PATTERN:",pattern
	addfiles+=Dir[pattern].select{|f|not f=~/swig.h/}
}

generateInterfaceFile(myInput,files,addfiles)

# old implementation


classList=[]
deriveList={} # x=>y :x is child of y
rubyClasses={} # all, which are derived from AGRubyObject
class2File={}


# take RubyObject as base for RubyObjects :-)
# so simply search for classes deriveListd from this!

files.each{|fn|
	g=File.open(fn)
	cn=""
	g.each{|a|
		abak=a
		a.gsub!("AGEXPORT","")
		a.gsub!("EXPORT","")
		
		if a =~ /^class.*/ then
			cn=a.gsub("class ","").gsub(/[:;].*/,"").gsub(/\n/,"").gsub(" ","")
			if cn=~/^[A-Z].*/
				classList+=[cn]
				if a=~ /.*public.*/ then
					pn=a.gsub(/.*public /,"").gsub(/\n/,"")
					deriveList[cn]=pn
					rubyClasses[cn]=false
					rubyClasses[pn]=false
					class2File[cn]=fn
				elsif not a=~/;/ then
					deriveList[cn]=nil
				end
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

file=File.open(myInput.markerName,"w")

# ok, first marking is included
parsedClasses.getMyRubyClasses.each {|x|
	#if y then
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
	#end
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

myClasses=[]

deriveList.each{|b,a|
	if rubyClasses.member?(b) || b=="AGRubyObject"
		myClasses << b
	end
}

myClasses=myClasses.sort.uniq

puts "rubyClasses:",rubyClasses
puts "myClasses:",myClasses

file.puts <<EOT
%{
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
	       std::cout<<"test:"<<t<<std::endl;
         if(t)
           return t;
      }
   return 0;
 }
%}
DYNAMIC_CAST(SWIGTYPE_p_#{k}, #{k}_dynamic_cast);
EOT
}

deriveList.each{|b,a|
	if rubyClasses.member?(b) and allRubyClasses.member?(a)
		# for each pair generate a casting function and register it into agCastFunctions
file.puts <<EOT
%{
swig_type_info* #{a}2#{b}cast(void **p)
{
        #{a}*a=(#{a}*)(*p);
        #{b}*b=dynamic_cast<#{b}*>(a);
				printf("TRY CAST #{a} 2 #{b} : %lx\\n",b);
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



