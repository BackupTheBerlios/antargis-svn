#!/usr/bin/env ruby

class AntFire<AntMyEntity
	def initialize(p=AGVector2.new(0,0))
		super
		mp=AGVector3.new(0,0,0)
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/fire.ant2",0.3,"data/textures/models/fire.png"),AGVector4.new(0,0,0),0)
		setMesh(mesh)
		addMesh(Smoke.new(getMap.getScene,4),mp)
		smoke=Smoke.new(getMap.getScene,40)
		smoke.setFire(true)
		smoke.setMaxTime(0.8)
		addMesh(smoke,mp)
		setPos(AGVector2.new(p.x,p.y))
		@enabled=true
	end
	def disable
		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/fire.ant2",0.3,"data/textures/models/fire2.png"),AGVector4.new(0,0,0),0))
		@enabled=false
	end
	def xmlName
		"antFire"
	end
	def loadXML(n)
		disable  # disable because hero makes new fire anyway
	end
	def eventNoJob
		if @enabled
			newRestJob(10)
		else
			if @dead
				$map.removeEntity(self)
			else
				@dead=true
				newRestJob(10)
			end
		end
	end
end