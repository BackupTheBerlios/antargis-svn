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
