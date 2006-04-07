#!/usr/bin/env ruby

class AntBoat<AntMyEntity
	def initialize(p=AGVector2.new(0,0))
		super
		setupMesh
	end
	def xmlName
		"antBoat"
	end
# 	def eventNoJob
# 		if @enabled
# 			newRestJob(10)
# 		else
# 			if @dead
# 				$map.removeEntity(self)
# 			else
# 				@dead=true
# 				newRestJob(10)
# 			end
# 		end
# 	end
private
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/boat_simple.ant2",0.2,"data/textures/models/boat_simple.png"),AGVector4.new(0,0,0),0)
		setMesh(mesh)
	end
end