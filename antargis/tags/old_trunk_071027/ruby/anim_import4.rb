#!/usr/bin/env ruby

#
# anim_import4.rb imports animated mesh in milkshape-text-format into BoA's internal ant4-format
# (which is more or less the same as milkshape-text-format in binary form)
#

require 'antargisgui.rb'

if ARGV == nil or ARGV.length<2
	puts "call: anim_import.rb mesh.txt mesh.ant4"
	exit
end
infilename=ARGV[0]
outfilename=ARGV[1]

$infile=File.open(infilename)

def getline
	if $infile.eof
		throw "getline reached eof"
	end
	line=$infile.readline
	if line=~/\/\/.*/ or line=="\n"
		return getline
	else
		return line.gsub("\n","")
	end
		
end

def assert(cond)
	if not cond
		throw "assertion failed"
	end
end

class Vertex
	def initialize(pos,tex,b)
		@pos=pos
		@tex=tex
		@bone=b
	end
	def x
		@pos.x
	end
	def y
		@pos.y
	end
	def z
		@pos.z
	end
	def tx
		@tex.x
	end
	def ty
		1-@tex.y
	end
	def bone
		@bone
	end
end

class Triangle
	def initialize(v0,v1,v2,n0,n1,n2,g)
		@group=g
		@v0=v0
		@v1=v1
		@v2=v2
		@n0=n0
		@n1=n1
		@n2=n2
	end
	def fvertices
		[f0,f1,f2]
	end
	def f0
		[$vertices[@v0],$normals[@n0]]
	end
	def f1
		[$vertices[@v1],$normals[@n1]]
	end
	def f2
		[$vertices[@v2],$normals[@n2]]
	end
end

class Frame
	def initialize(t,pv)
		@time=t
		@v=pv
	end
	attr_reader :time, :v
end

class Bone
	def initialize(p,r,par)
		@pos=p
		@rot=r
		@rframes=[]
		@tframes=[]
		if par
			@parent=par
		else
			@parent=-1
		end
	end
	def addRFrame(f)
		@rframes.push(f)
	end
	def addTFrame(f)
		@tframes.push(f)
	end
	attr_reader :pos, :rot, :rframes, :parent, :tframes
end

# read frames
line=getline
assert(line=~/Frames.*/)
line.sub!("Frames: ","")
$frames=line.to_i

line=getline
assert(line=~/Frame.*/)
line.sub!("Frame: ","")
$curframe=line.to_i

line=getline
assert(line=~/Meshes.*/)
line.sub!("Meshes: ","")
$meshes=line.to_i

$vertices=[]
$normals=[]
$tris=[]

$vstart=0
$nstart=0
for mesh in 0..($meshes-1)

	line=getline
	$meshname,flags,$material=line.split(" ")
	$meshname.gsub!("\"","")
	
	# read vertices
	line=getline
	$numvertices=line.to_i
	for i in 0..($numvertices-1)
		line=getline
		flags,x,y,z,u,v,bone=line.split(" ").collect{|x|x.to_f}
		bone=bone.to_i
		$vertices.push(Vertex.new(AGVector3.new(x,y,z),AGVector2.new(u,v),bone))
	end
	
	#read normals
	$numnormals=getline.to_i
	for i in 0..($numnormals-1)
		line=getline
		x,y,z=line.split(" ").collect{|x|x.to_f}
		$normals.push(AGVector3.new(x,y,z))
	end
	
	#read triangles
	$numtris=getline.to_i
	for i in 0..($numtris-1)
		line=getline
		flag,i0,i1,i2,n0,n1,n2,g=line.split(" ").collect{|x|x.to_i}
		$tris.push(Triangle.new(i0+$vstart, i1+$vstart, i2+$vstart, n0+$nstart, n1+$nstart, n2+$nstart ,g))
	end
	$vstart=$vertices.length
	$nstart=$normals.length
end
# read materials =1  !!!
line=getline
puts line
line.gsub!("Materials: ","")
$mats=line.to_i
for mat in 1..$mats
	#assert(line=="Materials: 1")
	line=getline
	assert(line=~/".*"/)
	
	# ambient
	line=getline
	r,g,b,a=line.split(" ").collect{|x|x.to_f}
	$ambient=AGVector4.new(r,g,b,a)
	
	# diffuse
	line=getline
	r,g,b,a=line.split(" ").collect{|x|x.to_f}
	$diffuse=AGVector4.new(r,g,b,a)
	
	# specular
	line=getline
	r,g,b,a=line.split(" ").collect{|x|x.to_f}
	$specular=AGVector4.new(r,g,b,a)
	
	# emissive
	line=getline
	r,g,b,a=line.split(" ").collect{|x|x.to_f}
	$emissive=AGVector4.new(r,g,b,a)
	
	$shininess=getline
	$transparency=getline
	
	$colortex=getline.gsub("\"","")
	$alphatex=getline.gsub("\"","")
end
line=getline
puts line
assert(line=~/Bones.*/)
$numbones=line.gsub(/.* /,"").to_i

puts "Bones #:"+$numbones.to_s
$bones=[]
$bonenames={}
for i in 0..($numbones-1)
	name=getline.gsub("\"","")
	parent=getline.gsub("\"","")
	$bonenames[name]=i
	flags,x,y,z,rx,ry,rz=getline.split(" ").collect{|a|a.to_f}
	bone=Bone.new(AGVector3.new(x,y,z),AGVector3.new(rx,ry,rz),$bonenames[parent])
	
	poskeys=getline.to_i
	pkeys={}
	rkeys={}
	for j in 1..poskeys
		t,x,y,z = getline.split(" ").collect{|a|a.to_f}
		bone.addTFrame(Frame.new(t,AGVector3.new(x,y,z)))
	end
	rotkeys=getline.to_i
	for j in 1..rotkeys
		t,x,y,z = getline.split(" ").collect{|a|a.to_f}
		bone.addRFrame(Frame.new(t,AGVector3.new(x,y,z)))
	end
	
	$bones.push(bone)
end

begin
	f=File.open(ARGV[1],"r")
	puts "File '"+ARGV[1]+"' already exists!"
	puts "Please give an other name!"
	exit
rescue
end

file=File.new(ARGV[1],"w")

fvertices=[]

$tris.each{|t|
	fvertices+=t.fvertices
}
fvertices.uniq!
puts "distinct verts:"+$vertices.length.to_s
puts "distinct fverts:"+fvertices.length.to_s
# first give vertices

file.print [fvertices.length].pack("i")

fvertices.each{|f|
	a=[f[0].x,f[0].y,f[0].z,f[0].tx,f[0].ty,f[0].bone,f[1].x,f[1].y,f[1].z]
	s=a.pack("fffffifff")
	file.print s
}

file.print [$tris.length].pack("i")

$tris.each{|t|
	a=[]
	t.fvertices.each{|v|
		i=fvertices.index(v)
		a.push(i)
	}
	file.print a.pack("iii")
}

file.print [$frames,$bones.length].pack("ii")
$bones.each{|b|
	a=[b.pos.x,b.pos.y,b.pos.z, b.rot.x,b.rot.y,b.rot.z,b.parent]
	file.print a.pack("ffffffi")
	
	# rot frames
	fs=b.rframes
	file.print [fs.length].pack("i")
	fs.each{|f|
		file.print [f.time,f.v.x, f.v.y, f.v.z].pack("ffff")
	}
	
	# translation frames
	fs=b.tframes
	file.print [fs.length].pack("i")
	fs.each{|f|
		file.print [f.time,f.v.x, f.v.y, f.v.z].pack("ffff")
	}
}

file.close


# file format "ant3" is
# 1)number of vertices in int32
# 2)vertices x,y,z,tx,ty,bone,nx,ny,nz all float32 but bone (int32)
# 3)number of triangles in int32
# 4)triangles v0,v1,v2 (all int32)
# 5)number of bones  in int32
# 6)bones x,y,z,rx,ry,rz (all float32) - positioned at x,y,z - rot-normal at rx,ry,rz
# 7)number of rot-keyframe in int32 per bone
# 8)rot-keyframes t,[(rx,ry,rz) ]
# 9)number of translation-keyframe in int32 per bone
# 10)translation-keyframes t,[(rx,ry,rz) ]
# so 7-10 is repeated for each bone
# end
