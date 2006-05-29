#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

# parse ruby files first
files=`find -name "*.rb"`.split("\n")

strs=[]

files.each{|file|
	File.open(file).each_line{|line|
		line.scan(/_\([^\)]*\)/){|x|
			if x[2..2]=="'" || x[2..2]=='"'
				a=x.scan(/('[^']*'|"[^"]*")/)
				if a.length>0
					str=a.to_s[1..-2]
					if str!=""
						strs.push(str)
					end
				end
			end
		}
	}
}

def checkNode(node)
	res=[]
	node.getChildren.each{|c|
		res+=checkNode(c)
	}
	caption=node.get("caption")
	if caption!=""
		res.push(caption)
	end
	res
end

files=`find -name "*.xml"|grep -v skat`.split("\n")

files.each{|file|
	#puts file
	d=Document.new(file)
	strs+=checkNode(d.root)

}

strs.uniq!

puts strs.join("\n")	