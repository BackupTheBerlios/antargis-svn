#!/usr/bin/env ruby

class AntWorkshop<AntHouse
	def init
		super
		setType("workshop")
		@lastBirth=0
		raise 1
#		mesh=Mesh.new(getMeshData("data/models/workshop.ant",3),AGVector4.new(0,0,0),-30)
		mesh=Mesh.new(getMeshData("data/models/farm.ant",2,"data/textures/models/tower_tex.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end


	def xmlName
		"antWorkshop"
	end
end