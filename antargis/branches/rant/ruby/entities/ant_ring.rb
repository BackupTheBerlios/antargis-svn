module RingData
	@@ringdata=nil
	def RingData.getRingData(w=1)
		@@ringdata||=[]
		if @@ringdata[w]
			return @@ringdata[w]
		end
	
		opt=MeshOptimizer.new
		mv0=MeshVertex.new
		mv1=MeshVertex.new
		mv2=MeshVertex.new
		mv3=MeshVertex.new
		
		c=AGVector4.new(1,1,1,1) #0.6,0.7,1,1) #0.9)
		mv0.c=c
		mv1.c=c
		mv2.c=c
		mv3.c=c
		
		mv0.t=AGVector2.new(0,0)
		mv1.t=AGVector2.new(1,0)
		mv2.t=AGVector2.new(1,1)
		mv3.t=AGVector2.new(0,1)
		
		#w=size
		h=0.1
		
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
		
		@@ringdata[w]=MeshData.new(opt.getArray,"data/textures/ring.png",false)
		@@ringdata[w].setOverdraw(true)
		@@ringdata[w].setColors(false)
		@@ringdata[w].setPickable(false)
		@@ringdata[w].setLighting(false)
		return @@ringdata[w]
	end
end

class ColoredMesh<Mesh
	def initialize(scene,data,pos,angle)
		super
		@t=0
		@c0=AGVector4.new(0.6,0.6,0.6,1)
		@c1=AGVector4.new(0.8,0.8,0.8,1)
	end
	def setRingColor(c)
		@c0=c
		@c1=c*1.2
		@c1.setW(1)
	end
	def advance(time)
		super
		@t+=time
		v=(Math::cos(@t*5)+1)/2
		setColor(@c0*v+@c1*(1-v))
	end
end


def makeRingMesh
	return nil if getMap.getScene.nil?
	return AntModels.createModel(:sack) if not opengl # FIXME
	mesh=ColoredMesh.new(getMap.getScene,RingData.getRingData,AGVector4.new(0,0,0,0),0)
	mesh.setOrder(RING_Z)
	return mesh
end

def makeBigRingMesh
	return nil if getMap.getScene.nil?
	return AntModels.createModel(:sack) if not opengl # FIXME
	mesh=ColoredMesh.new(getMap.getScene,RingData.getRingData(4),AGVector4.new(0,0,0,0),0)
	mesh.setOrder(RING_Z)
	return mesh
end



