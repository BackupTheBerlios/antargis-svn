#!/usr/bin/env

def getFirMeshData
	if $firdata
		return $firdata
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
	
	# add trunk
	radl=0.2
	radu=0.01
	height=4
	segments=8
	for ai in 0..(segments-1)
		a0=ai*Math::PI*2/segments
		a1=(ai+1)*Math::PI*2/segments
		
		xu=Math::sin(a0)*radu
		yu=Math::cos(a0)*radu
		xl=Math::sin(a0)*radl
		yl=Math::cos(a0)*radl
		
		p0=AGVector4.new(xl,yl,0,1)
		p1=AGVector4.new(xu,yu,height,1)
		
		xu=Math::sin(a1)*radu
		yu=Math::cos(a1)*radu
		xl=Math::sin(a1)*radl
		yl=Math::cos(a1)*radl
		
		p2=AGVector4.new(xl,yl,0,1)
		p3=AGVector4.new(xu,yu,height,1)
		
		n0=p0.dim3.normalized
		n1=p1.dim3.normalized
		n2=AGVector3.new(xl,yl).normalized
		n3=AGVector3.new(xu,yu).normalized
		
		tn=AGVector2.new(0.5,0.5)
		
		t0=AGVector2.new(0,0)+tn
		t1=AGVector2.new(0.5,0)+tn
		t3=AGVector2.new(0.5,0.5)+tn
		t2=AGVector2.new(0,0.5)+tn
		
		mv0.v=p0
		mv1.v=p1
		mv2.v=p2
		mv3.v=p3
		
		mv0.n=n0
		mv1.n=n1
		mv2.n=n2
		mv3.n=n3
		
		mv0.t=t0
		mv1.t=t1
		mv2.t=t2
		mv3.t=t3
		
		opt.add(mv0)
		opt.add(mv1)
		opt.add(mv2)
		
		opt.add(mv0)
		opt.add(mv2)
		opt.add(mv3)
	end
	
	
	
	
	$firdata=MeshData.new(opt.getArray,"data/textures/models/fir_complete.png")
	
	return $firdata
end

def makeFirMesh
	return Mesh.new(getFirMeshData,AGVector4.new(0,0,0,0),0)
end