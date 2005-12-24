#!/usr/bin/env ruby

def getGrassMeshData(size,many=4)
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
	
	for i in 1..many
	
	mv0.c=AGVector4.new(1,1,1,1)
	mv1.c=AGVector4.new(1,1,1,1)
	mv2.c=AGVector4.new(1,1,1,1)
	mv3.c=AGVector4.new(1,1,1,1)
	
	mv0.t=AGVector2.new(0,0)
	mv1.t=AGVector2.new(1,0)
	mv2.t=AGVector2.new(1,1)
	mv3.t=AGVector2.new(0,1)
	
	w=size

	v0=AGVector4.new(-w,0,0)
	v1=AGVector4.new(w,0,0)
	v2=AGVector4.new(w,0,w*2)
	v3=AGVector4.new(-w,0,w*2)
	
	if i==1
		vadd=AGVector4.new(0,0,0,0)
	else
		vadd=AGVector4.new(rand*2,rand*2,0,0)
	end
	
	mv0.v=v0+vadd
	mv1.v=v1+vadd
	mv2.v=v2+vadd
	mv3.v=v3+vadd
	
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
	
	mv0.v=turn*v0+vadd
	mv1.v=turn*v1+vadd
	mv2.v=turn*v2+vadd
	mv3.v=turn*v3+vadd
	
	opt.add(mv0)
	opt.add(mv1)
	opt.add(mv2)
	
	opt.add(mv0)
	opt.add(mv2)
	opt.add(mv3)
	
	turn=AGMatrix4.new(-1*Math::PI/3,AGVector3.new(0,0,1))
	
	mv0.v=turn*v0+vadd
	mv1.v=turn*v1+vadd
	mv2.v=turn*v2+vadd
	mv3.v=turn*v3+vadd
	
	opt.add(mv0)
	opt.add(mv1)
	opt.add(mv2)
	
	opt.add(mv0)
	opt.add(mv2)
	opt.add(mv3)
	end
	$grassdata[size]=MeshData.new(opt.getArray,"data/textures/models/high_grass2.png",true) #false)
	$grassdata[size].setLighting(false)
	#$grassdata[size]=MeshData.new(opt.getArray,"data/textures/models/flower1.png",false)
	
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
