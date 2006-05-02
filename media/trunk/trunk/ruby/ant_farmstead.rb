#!/usr/bin/env ruby

class AntFarmStead<AntHouse
	def initialize
		super
		setProvide("farmstead",true)
		@lastBirth=0
		setDirection(-20)
	end
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/livinghouse.ant2",0.16,"data/textures/models/livinghouse.png"),AGVector4.new(0,0,0),-20)
		setMesh(mesh)
	end

	
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

	def needed
		if resource.get("food")<15
			return "food"
		else
			return nil
		end
	end

end

