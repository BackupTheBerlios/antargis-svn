#!/usr/bin/env

$genNewTree=false

def makeTrunk(opt,p0,p1,r0,r1,segments)
	mv0=MeshVertex.new
	mv1=MeshVertex.new
	mv2=MeshVertex.new
	mv3=MeshVertex.new
	
	
	mv0.c=AGVector4.new(1,1,1,1)
	mv1.c=AGVector4.new(1,1,1,1)
	mv2.c=AGVector4.new(1,1,1,1)
	mv3.c=AGVector4.new(1,1,1,1)

	
	md0=(p1-p0).dim3.normalized
	x=AGVector3.new(md0.y,md0.z,md0.x)
	md1=md0%x
	puts "MD0",md0
	puts "MD1",md1
	md2=md1%md0
	puts "MD2",md2
	
	d0=AGVector4.new(md0,0)
	d1=AGVector4.new(md1,0)
	d2=AGVector4.new(md2,0)
	
	# add trunk
	radl=0.1
	radu=0.01
	height=6
	segments=8
	for ai in 0..(segments-1)
		a0=ai*Math::PI*2/segments
		a1=(ai+1)*Math::PI*2/segments
	
		mv0.v=p0 + d1*Math::sin(a0)*r0 + d2*Math::cos(a0)*r0
		mv1.v=p1 + d1*Math::sin(a0)*r1 + d2*Math::cos(a0)*r1
		
		mv2.v=p0 + d1*Math::sin(a1)*r0 + d2*Math::cos(a1)*r0
		mv3.v=p1 + d1*Math::sin(a1)*r1 + d2*Math::cos(a1)*r1
		
		mv0.n=(mv0.v - p0).dim3.normalized
		mv1.n=(mv1.v - p1).dim3.normalized
		mv2.n=(mv2.v - p0).dim3.normalized
		mv3.n=(mv3.v - p1).dim3.normalized
			
		tn=AGVector2.new(0.5,0.5)
		
		t0=AGVector2.new(0,0)+tn
		t1=AGVector2.new(0.5,0)+tn
		t3=AGVector2.new(0.5,0.5)+tn
		t2=AGVector2.new(0,0.5)+tn
		
		mv0.t=t0
		mv1.t=t1
		mv2.t=t2
		mv3.t=t3
		
		opt.add(mv2)
		opt.add(mv1)
		opt.add(mv0)
		
		opt.add(mv1)
		opt.add(mv2)
		opt.add(mv3)
	end
end

class Twig
	def initialize(p0,p1,r0,r1)
		@p0=p0
		@p1=p1
		@r0=r0
		@r1=r1
		@len=(@p1-@p0).length3
		
		@lenvar=0.2
		@cavar=0.3
		@simple=false
	end
	def setR1(r1)
		@r1=r1
	end
	def make(opt,segs)
		if @simple
			makeTrunk(opt,@p0,@p1,@r0,@r1,segs)
		else
			d=(@p1-@p0).dim3
			x=AGVector3.new(@p0.z,@p0.x,@p0.y)
			o=(d%x).normalized
			px=(@p0+@p1)*0.5+AGVector4.new(o,0)*d.length*0.15
			
			r=(@r0+@r1)*0.5
			makeTrunk(opt,@p0,px,@r0,r,segs)
			makeTrunk(opt,px,@p1,r,@r1,segs)
			
		end
	end
	
	def makeLeaves(opt)
		d=(@p1-@p0).dim3
		len=d.length
		
		width=len*1
		
		b=(d%AGVector3.new(0,0,1)).normalized
		
		pb=@p1+AGVector4.new(d*0.7,0)
		
		pa0=@p0+AGVector4.new(b*width,0)
		pa1=@p0-AGVector4.new(b*width,0)
		
		pb0=pb+AGVector4.new(b*width,0)
		pb1=pb-AGVector4.new(b*width,0)
		
		n=(d%b).normalized
		n*=-1
		
		mv0=MeshVertex.new
		mv1=MeshVertex.new
		mv2=MeshVertex.new
		mv3=MeshVertex.new
	
		mv0.c=AGVector4.new(1,1,1,1)
		mv1.c=AGVector4.new(1,1,1,1)
		mv2.c=AGVector4.new(1,1,1,1)
		mv3.c=AGVector4.new(1,1,1,1)
		
		mv0.v=pa0
		mv1.v=pa1
		mv2.v=pb1
		mv3.v=pb0
		
		mv0.n=n
		mv1.n=n
		mv2.n=n
		mv3.n=n
		
		mv2.t=AGVector2.new(0,0)
		mv3.t=AGVector2.new(0.5,0)
		mv0.t=AGVector2.new(0.5,0.5)
		mv1.t=AGVector2.new(0,0.5)

		# front
		opt.add(mv2)
		opt.add(mv1)
		opt.add(mv0)
		
		opt.add(mv3)
		opt.add(mv2)
		opt.add(mv0)
		
		# back
		#opt.add(mv0)
		#opt.add(mv1)
		#opt.add(mv2)
		
		#opt.add(mv0)
		#opt.add(mv2)
		#opt.add(mv3)
	end
	
	def getChildren(num,root=false)
		
		children=[]
		
		d=(@p1-@p0).dim3.normalized
		x=AGVector3.new(d.z,d.x,d.y)
		o=d%x
		o2=d%o
		
		
		turn1=AGMatrix4.new(Math::PI/3,o)
		if root
			turn1=AGMatrix4.new(Math::PI/5,o)
		end
		
		np1=@p1*0.9+@p0*0.1
		
		for i in 1..num
			ca=i*Math::PI*2/num+rand*@cavar
			puts "CA:",ca
			turn2=AGMatrix4.new(ca,d)
			turn=turn2*turn1
			
			np2=np1+turn*(@p1-@p0)*(0.4+rand*@lenvar)
			
			children.push(Twig.new(np1,np2,@r1*0.8,@r1*0.6))
		end
		
		
		children.push(Twig.new(@p1,@p1+AGVector4.new((@p1-@p0).dim3*0.6,0),@r1*0.8,@r1*0.6))
		
		return children
	end
	
end

def getAppleTreeMeshData(type="apple")
	if not $appletreedata
		$appletreedata={}
	end


	case type
		when "apple"
			texture="data/textures/models/fir_complete.png"
		when "birch"
			texture="data/textures/models/birch_complete.png"
	end
	if not ($appletreedata[type] or $genNewTree)
		# simply comment this line to generate a new tree in "bla.ant2"
		# then copy it to whatever file you want.
		$appletreedata[type]=MeshData.new("data/models/tree1.ant2",1,texture)
		$appletreedata[type].setTransparent(true)
		#$appletreedata[type].setOverdraw(true) #Transparent(true)
		puts "TREE_TRIANGLES:"
		puts $appletreedata[type].getTriangles
		#raise 1
		#$appletreedata=MeshData.new("data/models/tree1.ant2",1,"data/textures/models/birch_complete.png")
	end	

	if $appletreedata[type]
		return $appletreedata[type]
	end
	
	
	
	puts "MAKEAPPLEMESH"
	opt=MeshOptimizer.new
	
	# add trunk
	radl=0.1
	radu=0.01
	height=6
	bheight=2.3
	segments=8
	
		
	#makeTrunk(opt,AGVector4.new(-1,0,0),AGVector4.new(-1,0,1),0.3,0.1,5)
	
	twig=Twig.new(AGVector4.new(0,0,0),AGVector4.new(0,0,bheight),0.2,0.1)
	
	twigs=[twig]
	for i in 0..1
		nt=[]
		twigs.each{|t|
			t.make(opt,4)
			nt+=t.getChildren(5+(rand()*2).to_i+i*3)
		}
		old=twigs
		twigs=nt
	end
	
	(twigs+old).each{|t|
		t.makeLeaves(opt)
	}
		
	# make roots
	trunk=Twig.new(AGVector4.new(0,0,1.3),AGVector4.new(0,0,0.1),0.18,0.15)
	trunk.getChildren(5,true).each{|c|
		c.setR1(0.05)
		c.make(opt,4)
	}
	
	
	$appletreedata[type]=MeshData.new(opt.getArray,texture)
	#$appletreedata=MeshData.new(opt.getArray,"data/textures/models/birch_complete.png")
	$appletreedata[type].save("bla.ant2")
	#$appletreedata.setAlpha(true)
	puts "TRIS:"
	puts $appletreedata[type].getTriangles
	$appletreedata[type].setTransparent(true)
	return $appletreedata[type]
end

def makeAppleTreeMesh(angle=0)
	return Mesh.new(getAppleTreeMeshData,AGVector4.new(0,0,0,0),angle)
end

def makeBirchTreeMesh(angle=0)
	return Mesh.new(getAppleTreeMeshData("birch"),AGVector4.new(0,0,0,0),angle)
end
