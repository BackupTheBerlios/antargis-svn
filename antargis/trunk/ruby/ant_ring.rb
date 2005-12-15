#!/usr/bin/env ruby

def getRingData
	if $ringdata
		return $ringdata
	end

	size=1
		
	puts "MAKEFIRMESH"
	opt=MeshOptimizer.new
	mv0=MeshVertex.new
	mv1=MeshVertex.new
	mv2=MeshVertex.new
	mv3=MeshVertex.new
	
	c=AGVector4.new(0.6,0.7,1,1) #0.9)
	mv0.c=c
	mv1.c=c
	mv2.c=c
	mv3.c=c
	
	mv0.t=AGVector2.new(0,0)
	mv1.t=AGVector2.new(1,0)
	mv2.t=AGVector2.new(1,1)
	mv3.t=AGVector2.new(0,1)
	
	w=size
	h=0.4
	
	mv0.v=AGVector4.new(-w,-w,h)
	mv1.v=AGVector4.new(w,-w,h)
	mv2.v=AGVector4.new(w,w,h)
	mv3.v=AGVector4.new(-w,w,h)
	
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
	
	$ringdata=MeshData.new(opt.getArray,"data/textures/ring.png",false)
	$ringdata.setOverdraw(true)
	
	return $ringdata
end

def makeRingMesh
	mesh=Mesh.new(getRingData,AGVector4.new(0,0,0,0),0)
	mesh.setOrder(DECAL_Z)
	return mesh
end



