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

clist=[]
derive={} # x=>y :x is child of y
rlist={} # all, which have a mRUBY member

files=ARGV
puts "FILES:"
puts files.join(" ")

# take RubyObject as base for RubyObjects :-)
# so simply search for classes derived from this!

puts ""
files.each{|fn|
	g=File.open(fn)
	cn=""
	g.each{|a|
		if a =~ /^class.*/ then
			cn=a.gsub("class ","").gsub(/[:;].*/,"").gsub(/\n/,"")
			clist+=[cn]
			if a=~ /.*public.*/ then
				pn=a.gsub(/.*public /,"").gsub(/\n/,"")
				derive[cn]=pn
				rlist[cn]=false
				rlist[pn]=false
			end
		end
	
	}
}

rlist["RubyObject"]=true

rlist.each{|a,b|
	puts "rlist:"+a+":"+b.to_s
}

puts "----"

# check for children of AGWidget
changed=true
while changed do
	changed=false
	derive.each {|x,y|
		if rlist[y] and rlist[x]==false then
			rlist[x]=true
			changed=true
		end
	}
end

rlist.each{|a,b|
	puts "rlist:"+a+":"+b.to_s
}

file=File.open("nantmarker.hh","w")

# ok, first marking is included
rlist.each {|x,y|
	if y then
		file.puts "%exception "+x+"::"+x+" {"
		file.puts "	$action"
		file.puts "	result->mRUBY=self;"
		file.puts "  result->mRubyObject=true;"
		#file.puts '  printf("registerant:'+x+'\n");'
		file.puts "}"
		file.puts "%markfunc "+x+" \"general_markfunc\""
	end
}

# now add typemap(out)

derivations={}
rlist.each{|x,y|
	puts "X:"+x
	derivations[x]=[]
}
derive.each{|x,y|
	puts x+" is child of "+y
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
				puts x+":"+old.length.to_s+" "+derivations[x].length.to_s
				if old.length<derivations[x].length
					changed=true
				end
			end
		}
	}
end
puts "---"
derivations.each{|a,cs|
	puts a+":"+cs.join(",")
}
puts "---"

derive.keys.each{|s|
	if rlist[s]
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
					puts s+"  "+a+" "+i.to_s
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
	end
}

