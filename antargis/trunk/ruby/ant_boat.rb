#!/usr/bin/env ruby

def getBoatMeshData
	getMeshData("data/models/boat_simple.ant2",0.12,"data/textures/models/boat_simple.png")
end

class AntBoat<AntMyEntity
	def initialize(p=AGVector2.new(0,0))
		super
		setupMesh
	end
	def xmlName
		"antBoat"
	end
private
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getBoatMeshData,AGVector4.new(0,0,0),0)
		setMesh(mesh)
	end
end