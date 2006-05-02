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
	radl=0.1
	radu=0.01
	height=6
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
		
		opt.add(mv2)
		opt.add(mv1)
		opt.add(mv0)
		
		opt.add(mv3)
		opt.add(mv2)
		opt.add(mv0)
	end

	# greenery
	rot=AGMatrix4.new(44.0*Math::PI/180.0,AGVector3.new(0,0,1))
	current=AGMatrix4.new
	y=height*0.4
	ys=0.05
	while y<height
		current=current*rot
		
		v1=AGVector4.new(0,2.4*(1-y/height),y-0.5)
		v2=AGVector4.new(-0.9*(1-y/height),0,y)
		v3=AGVector4.new(0.9*(1-y/height),0,y)
		
		v1=current*v1
		v2=current*v2
		v3=current*v3
		
		n1=v2-v1
		n2=v3-v1
		
		ntmp=n2%n1
		
		n=ntmp.dim3
		n.normalize
		
		n*=-1
		
		mv0.v=v1
		mv1.v=v2
		mv2.v=v3
		
		tn=AGVector2.new(0,0.5)
		
		mv0.t=AGVector2.new(0.5,0)*0.5+tn
		mv1.t=AGVector2.new(0,1)*0.5+tn
		mv2.t=AGVector2.new(1,1)*0.5+tn
		
		mv0.n=n
		mv1.n=n
		mv2.n=n
		
		opt.add(mv0)
		opt.add(mv1)
		opt.add(mv2)
		
		opt.add(mv2) # backface
		opt.add(mv1) # backface
		opt.add(mv0) # backface
		
		y+=ys
	end
	
	
	
	
	$firdata=MeshData.new(opt.getArray,"data/textures/models/fir_complete.png")
	$firdata.setTransparent(true)
	return $firdata
end

def makeFirMesh
	return Mesh.new(getFirMeshData,AGVector4.new(0,0,0,0),0)
end