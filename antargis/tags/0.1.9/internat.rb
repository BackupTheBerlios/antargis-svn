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
	caption=node.get("text")
	if caption!=""
		res.push(caption)
	end
	res.collect{|s|s.gsub("\n","\\n")}
end

files=`find -name "*.xml"|grep -v skat`.split("\n")

files.each{|file|
	#puts file
	d=Document.new(file)
	strs+=checkNode(d.root)

}

# get old
`find -name "local_*.txt"`.split("\n").each{|f|
	File.open(f).each_line{|line|
		s=line.gsub(/;;.*/,"").gsub("\n","")
		#puts "S:#{s}!"
		strs.push(s)
	}
}

strs.sort!
strs.uniq!

puts strs.join("\n")

f=File.open("local_new.txt","w")
f.puts strs.join(";;\n")
f.puts ";;\n"
