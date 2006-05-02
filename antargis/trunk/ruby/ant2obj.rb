#!/usr/bin/env ruby

infile=ARGV[0]

class Reader
	def initialize(c)
		@c=c
		@pos=0
		@vlen=[1].pack("v").length
		@elen=[1].pack("e").length
	end
	def readV
		v=@c[@pos..(@pos+@vlen)].unpack("v")
		@pos+=@vlen
		return v[0]
	end
	def readE
		v=@c[@pos..(@pos+@elen)].unpack("e")
		@pos+=@elen
		return v[0]
	end
end

def mreadv(c)
	l=[1].pack("v").length
	c=c[l..c.length]
end

puts infile

c=File.open(infile,"rb").read
r=Reader.new(c)
meshes=r.readV
puts meshes
faces=r.readV
puts faces
puts faces.class
(1..faces).each{|x|
	fsize=r.readV
	puts fsize
	(1..fsize).each{|v|
		vertex=[r.readE,r.readE,r.readE]
		normal=[r.readE,r.readE,r.readE]
		color=[r.readE,r.readE,r.readE]
		tex=[r.readE,r.readE]
		puts vertex.join("/")
	}
}
