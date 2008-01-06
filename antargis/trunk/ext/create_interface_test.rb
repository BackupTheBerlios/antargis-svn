#!/usr/bin/env ruby

Dir.chdir(File.split(__FILE__)[0])

require 'pp'
require 'sort_files.rb'

def vputs(*s)
end

def grep(pattern,filename)
	file=File.open(filename)
	result=(file.read=~pattern)
	file.close
	result
end

def getAllHeadersWithSwig
	Dir["*/*.h"].select{|file|grep(/INCLUDE_SWIG/,file)}.select{|file|not file=~/swig/}
end

files=getAllHeadersWithSwig

sorter=ParsedClasses.new(files,files)

files= sorter.getFileList

interfaceI=File.open("interface_new.i","w")

interfaceI.puts '%module(directors="1") antargis'
interfaceI.puts '%trackobjects;'

interfaceI.puts "%{"

files.each{|file|
	interfaceI.puts "#include \"#{file}\""
}
interfaceI.puts "%}"
interfaceI.puts '%include "common.i"'

Dir["*/template*.i"].each{|file|
	interfaceI.puts "%include \"#{file}\""
}


interfaceI.puts "/// markers"

sorter.getAllRubyClasses.each{|c|
	interfaceI.puts "%markfunc #{c} \"general_markfunc\""
	interfaceI.puts "%feature(\"director\") #{c};"
}

allRubyClasses=rubyClasses=sorter.getAllRubyClasses

myDerivations={}
sorter.deriveList.each{|child,parent|
	if parent
		myDerivations[parent]||=[]
		myDerivations[parent] << child
	end
}

#
# FIXME: return to multi-module - put derivation info into a map - together with casting function
#
myDerivations.each{|parent,children|
	interfaceI.puts "%{"
	interfaceI.puts "swig_type_info *#{parent}_dynamic_cast(void **p) {"
	interfaceI.puts "	#{parent} *a=(#{parent}*)(*p);"
	children.each{|child|
		interfaceI.puts "	{"
		interfaceI.puts "		#{child} *b=dynamic_cast<#{child}*>(a);"
		interfaceI.puts "		if(b) {"
		interfaceI.puts "			*p=(void*)b;"
		interfaceI.puts "			return SWIGTYPE_p_#{child};"
		interfaceI.puts "		}"
		interfaceI.puts "	}"
	}
	interfaceI.puts "	return 0;"
	interfaceI.puts "}"
	interfaceI.puts "%}"
	interfaceI.puts "%apply SWIGTYPE *DYNAMIC { #{parent} * };"
	#interfaceI.puts "%typemap(out) #{parent} * = SWIGTYPE *DYNAMIC;"
	#interfaceI.puts "%typemap(directorin) #{parent} * = SWIGTYPE *DYNAMIC;"
	interfaceI.puts "DYNAMIC_CAST(SWIGTYPE_p_#{parent}, #{parent}_dynamic_cast);"
}



files.each{|file|
	interfaceI.puts "%include \"#{file}\""
}


interfaceI.close

# FIXME
run="swig -Wall -DAGEXPORT -I/usr/include/SDL -ruby -c++ -o swig.cc -I../build/swig interface_new.i"

system(run)

