#!/usr/bin/env ruby

def getGrassMeshData(size)
	if not $grassdata
		$grassdata={}
	end
	if $grassdata[size]
		return $grassdata[size]
	end
	
	puts "MAKEFIRMESH"
	opt=MeshOptimizer.new
	mv0=MeshVertex.new
	mv1=MeshVertex.new
	mv2=MeshVertex.new
	mv3=MeshVertex.new
	
	
	mv0.c=AGVector4.new(1,1,1,1)
	mv1.c=AGVector4.new(1,1,1,1)
	mv2.c=AGVector4.new(1,1,1,1)
	mv3.c=AGVector4.new(1,1,1,1)
	
	mv0.t=AGVector2.new(0,0)
	mv1.t=AGVector2.new(1,0)
	mv2.t=AGVector2.new(1,1)
	mv3.t=AGVector2.new(0,1)
	
	w=size
	
	mv0.v=AGVector4.new(-w,0,0)
	mv1.v=AGVector4.new(w,0,0)
	mv2.v=AGVector4.new(w,0,w*2)
	mv3.v=AGVector4.new(-w,0,w*2)
	
	mv0.n=AGVector3.new(0,-1,0)
	mv1.n=AGVector3.new(0,-1,0)
	mv2.n=AGVector3.new(0,-1,0)
	mv3.n=AGVector3.new(0,-1,0)
	
	opt.add(mv0)
	opt.add(mv1)
	opt.add(mv2)
	
	opt.add(mv0)
	opt.add(mv2)
	opt.add(mv3)
	
	turn=AGMatrix4.new(Math::PI/3,AGVector3.new(0,0,1))
	
	mv0.v=turn*mv0.v
	mv1.v=turn*mv1.v
	mv2.v=turn*mv2.v
	mv3.v=turn*mv3.v
	
	opt.add(mv0)
	opt.add(mv1)
	opt.add(mv2)
	
	opt.add(mv0)
	opt.add(mv2)
	opt.add(mv3)
	
	#$grassdata[size]=MeshData.new(opt.getArray,"data/textures/models/high_grass.png",false)
	$grassdata[size]=MeshData.new(opt.getArray,"data/textures/models/flower1.png",false)
	
	return $grassdata[size]
end

def makeGrassMesh(size=0.4)
	return Mesh.new(getGrassMeshData(size),AGVector4.new(0,0,0,0),0)
end






class AntGrass<AntMyEntity
	def initialize()
		super(AGVector2.new(0,0))
		setType("grass")
		@size=rand*0.25+0.25
		setupMesh
	end
	def resourceChanged
		setupMesh
	end
	
	def xmlName
		return "antGrass"
	end
	
	private
	def setupMesh
		setMesh(makeGrassMesh(@size))
	end
end
