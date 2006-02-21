#!/usr/bin/env ruby

class AntFire<AntEntity
	def initialize(p)
		super
		mp=AGVector3.new(0,0,0)
		mesh=Mesh.new(getMeshData("data/models/fire.ant2",0.3,"data/textures/models/fire.png"),AGVector4.new(0,0,0),0)
		setMesh(mesh)
		addMesh(Smoke.new(4),mp)
		smoke=Smoke.new(40)
		smoke.setFire(true)
		smoke.setMaxTime(0.8)
		addMesh(smoke,mp)
		setPos(AGVector2.new(p.x,p.y))
	end
	def disable
		setMesh(Mesh.new(getMeshData("data/models/fire.ant2",0.3,"data/textures/models/fire2.png"),AGVector4.new(0,0,0),0))
	end
end