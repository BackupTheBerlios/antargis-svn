class AntFire<AntRubyEntity
	def initialize(p=AGVector2.new(0,0))
		super
		mp=AGVector3.new(0,0,0)
		setMesh(createModel(:fire,:on))
# 		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/fire.ant2",0.3,"data/textures/models/fire.png"),AGVector4.new(0,0,0),0)
# 		setMesh(mesh)
		@smokeMesh=Smoke.new(getMap.getScene,4)
		addMesh(@smokeMesh,mp)
		smoke=Smoke.new(getMap.getScene,40)
		smoke.setFire(true)
		smoke.setMaxTime(0.8)
		addMesh(smoke,mp)
		setPos(AGVector2.new(p.x,p.y))
		@enabled=true
	end
	def disable
		setMesh(createModel(:fire,:off))
		#setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/fire.ant2",0.3,"data/textures/models/fire2.png"),AGVector4.new(0,0,0),0))
		getMap.getScene.addNode(@smokeMesh) # FIXME: dirty hack - solve this another way!!!
		addMesh(@smokeMesh,AGVector3.new(0,0,0))
		@smokeMesh.setEnabled(false)
		@enabled=false
	end
	def loadXML(n)
		disable  # disable because hero makes new fire anyway
	end
	def eventNoJob
		if @enabled
			newRestJob(10)
		else
			if @dead
				getMap.removeEntity(self)
			else
				@dead=true
				newRestJob(10)
			end
		end
	end
end