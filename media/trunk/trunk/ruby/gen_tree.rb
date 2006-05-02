#!/usr/bin/env

def genTree
	f=loadFile("data/models/tree4.ant2")
	opt=loadFromText(f,true,0.5)
	
		r=4
		y0=11
		y1=12
		stepy=0.7
		for k in 0..9
			r=[1.3,1.3,1.5,2,2.5,3,3.5,4,4.3,4.5,4.5][k]
			if k<3
				stepy=0.5
			else
				stepy=1
			end
		
			segs=8
			for i in 0..(segs-1)
				a0=2*Math::PI/segs*i
				a1=2*Math::PI/segs*(i+1)
			
				mv0=MeshVertex.new
				mv1=MeshVertex.new
				mv2=MeshVertex.new
			
				mv0.c=AGVector4.new(1,1,1,1)
				mv1.c=AGVector4.new(1,1,1,1)
				mv2.c=AGVector4.new(1,1,1,1)
				
				mv0.v=AGVector4.new(Math::cos(a0)*r,Math::sin(a0)*r,y0,1)
				mv1.v=AGVector4.new(Math::cos(a1)*r,Math::sin(a1)*r,y0,1)
				mv2.v=AGVector4.new(0,0,y1,1)
				
				n=(mv0.v-mv1.v)%(mv2.v-mv1.v)
			
				n=n.dim3
				n=-n.normalized	
			
				mv0.n=n
				mv1.n=n
				mv2.n=n
				
				middle=AGVector2.new(0.5,0.75)
				
				mv0.t=AGVector2.new(Math::cos(a0+k),Math::sin(a0+k)*0.5)*0.5+middle
				mv1.t=AGVector2.new(Math::cos(a1+k),Math::sin(a1+k)*0.5)*0.5+middle
				mv2.t=middle
				
				# front
				opt.add(mv2)
				opt.add(mv1)
				opt.add(mv0)		
			end
			y0-=stepy
			y1-=stepy
		end
	
	va=opt.getArray
	data=MeshData.new(va,"data/textures/models/fir5.png",true)
	data.setTransparent(true)
	data.save("treetest2.ant2")
	puts data.getTriangles
	data
end


