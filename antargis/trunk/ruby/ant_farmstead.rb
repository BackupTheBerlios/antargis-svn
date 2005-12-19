#!/usr/bin/env ruby

class AntFarmStead<AntHouse
	def initialize
		super
		setType("farmstead")
		@lastBirth=0
		
	end
	def setupMesh
		mesh=Mesh.new(getMeshData("data/models/livinghouse.ant2",1.6,"data/textures/models/livinghouse.png"),AGVector4.new(0,0,0),-20)
		setMesh(mesh)
	end

	def xmlName
		"antFarmStead"
	end
	
	
	def checkBirth
		# only men, which are at home can add to birth rate
		@lastBirth+=[@atHome.length,30].min
		if @lastBirth>50 then
			man=AntNewMan.new
			getMap.insertEntity(man)
			man.setPos(getPos2D)
			getMap.endChange
			@lastBirth=-10*rand
		end
	end

	def needed
		if resource.get("food")<30
			return ["food","farm"]
		else
			return nil
		end
	end

end

