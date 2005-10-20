#!/usr/bin/env ruby

class AntFarmStead<AntHouse
	def init
		super
		setType("farmstead")
		@lastBirth=0
		
		mesh=Mesh.new(getMeshData("data/models/werkstatt.ant",30),AGVector4.new(0,0,0),-30)
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

	

end

