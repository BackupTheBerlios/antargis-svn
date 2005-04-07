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
derive={}
rlist={} # all, which have a mRUBY member


f=File.open("../include/antargisgui.h")
f.each {|x|

if x =~ /%.*/ then
	y=x.gsub(/.* "/,"").gsub(/"/,"").gsub(/\n/,"")
	fn="../include/"+y
	g=File.open(fn)
	cn=""
	found=false
	g.each{|a|
		if a =~ /^class.*/ then
#		 	puts a
			cn=a.gsub("class ","").gsub(/[:;].*/,"").gsub(/\n/,"")
#			puts cn
			clist+=[cn]
			if a=~ /.*public.*/ then
				pn=a.gsub(/.*public /,"").gsub(/\n/,"")
#				puts "pn:"+pn+"."
				derive[cn]=pn
			end
		end
		if a =~ /.*mRUBY.*/ then
			found=true
		end
	}
	rlist[cn]=found
end
}

# check for children of AGWidget
changed=true
while changed do
	changed=false
	derive.each {|x,y|
#		puts "derive:"+x+" "+y
		if rlist[y] and rlist[x]==false then
	#		puts "setting:"+x
			rlist[x]=true
			changed=true
		end
	}
end
#puts "hallo"
#puts rlist

rlist.each {|x,y|
	#puts x+y.to_s#x#"("+x+")"
	if y then
	puts "%exception "+x+"::"+x+" {"
	puts "	$action"
	puts "	result->mRUBY=self;"
  puts "	printf(\"register:"+x+"\\n\");"
  puts "}"
	end
}
