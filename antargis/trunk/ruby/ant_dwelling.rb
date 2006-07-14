#!/usr/bin/env ruby

# A dwelling has zero or more residents. Whenever there some residents they will reproduce themselves.
class AntDwelling<AntHouse
	def initialize
		super
		setProvide("dwelling",true)
		@lastBirth=0
		setDirection(-20)
	end

	# when my job is finished check for a new birth
	def eventJobFinished
		super
		checkBirth
	end

	# setup the mesh
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/livinghouse.ant2",0.16,"data/textures/models/livinghouse.png"),AGVector4.new(0,0,0),-20)
		setMesh(mesh)
	end

protected
	def neededStock
		{"food"=>25}
	end

	# check if we need some resources - residents will gather this
	def needed
		if resource.get("food")<15
			return "food"
		else
			return nil
		end
	end

	def houseType
		"dwelling"
	end
	
private
	def checkBirth
		# only men, which are at home can add to birth rate
		@lastBirth+=[@atHome.length,30].min
		if @lastBirth>50 then
			man=AntMan.new
			getMap.insertEntity(man)
			man.setPos(getPos2D)
			getMap.endChange
			@lastBirth=-10*rand
		end
	end
end

