#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# mk_marker_h.rb
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

#!/usr/bin/ruby

classList=[]
deriveList={} # x=>y :x is child of y
rubyClasses={} # all, which are derived from AGRubyObject


files=ARGV

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

file=File.open("nantmarker.hh","w")

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
		file.puts "  if($1->mRubyObject)"
		file.puts "    $result=$1->mRUBY;"
		file.puts "  else"
		file.puts "   {"
		file.puts "     if(false);"
		for i in 0..30
			derivations.each{|a,cs|
				if cs.length==i && derivations[s].member?(a)
					#puts s+"  "+a+" "+i.to_s
					file.puts "else if(dynamic_cast<#{a}*>(result))"
					file.puts "  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_#{a},0);"
				end
			}
		end
		file.puts "   else"
		file.puts "     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_#{s},0);"
		file.puts "   }"
		file.puts " }"
		file.puts " else vresult=Qnil;"
		file.puts "}"


	if true #false
		file.puts "%typemap(directorin) #{s}*{"
		file.puts " if($1)"
		file.puts " {"
		file.puts "  if($1->mRubyObject)"
		file.puts "    $input=$1->mRUBY;"
		file.puts "  else"
		file.puts "   {"
		file.puts "     if(false);"
		for i in 0..30
			derivations.each{|a,cs|
				if cs.length==i && derivations[s].member?(a)
					#puts s+"  "+a+" "+i.to_s
					file.puts "else if(dynamic_cast<#{a}*>($1))"
					file.puts "  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_#{a},0);"
				end
			}
		end
		file.puts "   else"
		file.puts "     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_#{s},0);"
		file.puts "   }"
		file.puts " }"
		file.puts " else $input=Qnil;"
		file.puts "}"
end

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
file.close

# now generate antargis.h
file=File.open("antargisgui.h","w")

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
