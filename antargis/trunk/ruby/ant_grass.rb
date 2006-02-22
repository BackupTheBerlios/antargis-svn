#!/usr/bin/env ruby

def getGrassMeshData(size,many=4,texture=nil,bush=false)
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
	
	if bush
		mv0.t=AGVector2.new(0,1-0)
		mv1.t=AGVector2.new(1,1-0)
		mv2.t=AGVector2.new(1,1-0.5)
		mv3.t=AGVector2.new(0,1-0.5)
	else
		mv0.t=AGVector2.new(0,1-0)
		mv1.t=AGVector2.new(1,1-0)
		mv2.t=AGVector2.new(1,1-1)
		mv3.t=AGVector2.new(0,1-1)
	end
	
	w=size
	h=w
	
	if bush
		h*=0.6
	end

	v0=AGVector4.new(-w,0,0)
	v1=AGVector4.new(w,0,0)
	v2=AGVector4.new(w,0,h*2)
	v3=AGVector4.new(-w,0,h*2)
	
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
# 	
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
# 	
 	opt.add(mv0)
 	opt.add(mv2)
 	opt.add(mv3)
	
	if bush
		mv0.t=AGVector2.new(0,1-0.5)
		mv1.t=AGVector2.new(1,1-0.5)
		mv2.t=AGVector2.new(1,1-1)
		mv3.t=AGVector2.new(0,1-1)
		mv0.n=AGVector3.new(0,0,1)
		mv1.n=AGVector3.new(0,0,1)
		mv2.n=AGVector3.new(0,0,1)
		mv3.n=AGVector3.new(0,0,1)
		mv0.v=AGVector4.new(-w,-w,h/3)
		mv1.v=AGVector4.new( w,-w,h/3)
		mv2.v=AGVector4.new( w, w,h/3)
		mv3.v=AGVector4.new(-w, w,h/3)
		opt.add(mv0)
		opt.add(mv1)
		opt.add(mv2)
		
		opt.add(mv0)
		opt.add(mv2)
		opt.add(mv3)
		mv0.v=AGVector4.new(-w*0.7,-w*0.7,h*0.7)
		mv1.v=AGVector4.new( w*0.7,-w*0.7,h*0.7)
		mv2.v=AGVector4.new( w*0.7, w*0.7,h*0.7)
		mv3.v=AGVector4.new(-w*0.7, w*0.7,h*0.7)
		opt.add(mv0)
		opt.add(mv1)
		opt.add(mv2)
		
		opt.add(mv0)
		opt.add(mv2)
		opt.add(mv3)
		
		mv0.v=AGVector4.new(-w/2,-w/2,h/2*3)
		mv1.v=AGVector4.new( w/2,-w/2,h/2*3)
		mv2.v=AGVector4.new( w/2, w/2,h/2*3)
		mv3.v=AGVector4.new(-w/2, w/2,h/2*3)
 		opt.add(mv0)
 		opt.add(mv1)
 		opt.add(mv2)
 		
 		opt.add(mv0)
 		opt.add(mv2)
 		opt.add(mv3)
	end
	
	
	end
	
	#$grassdata[size]=MeshData.new(opt.getArray,"data/textures/models/high_grass2.png",true) #false)
	$grassdata[size]=MeshData.new(opt.getArray,texture,true) #false) # last is shadow
	$grassdata[size].setTransparent(true)
	
	if nil
		$grassdata[size]=MeshData.new(opt.getArray,"data/textures/terrain/reed.png",true) #false)
		$grassdata[size].setLighting(false)
		#$grassdata[size].setOverdraw(true)
		$grassdata[size].setTransparent(true)
	end
	#$grassdata[size]=MeshData.new(opt.getArray,"data/textures/models/flower1.png",false)
	
	return $grassdata[size]
end

def makeGrassMesh(size=0.4)
	return Mesh.new(getGrassMeshData(size,4,"data/textures/models/high_grass2.png"),AGVector4.new(0,0,0,0),0)
end

def makeBushMesh(size=0.4)
	return Mesh.new(getGrassMeshData(size,1,"data/textures/models/bush5.png",true),AGVector4.new(0,0,0,0),0)
end




class AntGrass<AntMyEntity
	def initialize(high=false)
		super(AGVector2.new(0,0))
		setProvide("grass",true)
		if high
			@size=rand*0.25+0.5
		else
			@size=rand*0.25+0.25
		end
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

class AntHighGrass<AntGrass
	def initialize
		super(true)
	end
	def xmlName 
		"antHighGrass"
	end
end


class AntBush<AntMyEntity
	def initialize()
		super(AGVector2.new(0,0))
		setProvide("bush",true)
		@size=rand*0.25+0.25
		setupMesh
	end
	def resourceChanged
		setupMesh
	end
	
	def xmlName
		return "antBush"
	end
	
	private
	def setupMesh
		setMesh(makeBushMesh(@size*3))
	end
end
