#!/usr/bin/env ruby

class AntTower<AntHouse
	def initialize
		super
		setProvide("tower",true)
		
		#data=getMeshData("data/models/ruin.ant2",5,"data/textures/models/ruin.png")
		#data=getMeshData("data/models/old_monument.ant2",0.6,"data/textures/models/ruin.png")
		#data.texCoordFromPos(0.2)
		#mesh=Mesh.new(data,AGVector4.new(0,0,0),-30)
		#mesh=Mesh.new(getMeshData("data/models/tower.ant2",3,"data/textures/models/tower_new.png"),AGVector4.new(0,0,0),-30)
		#mesh=Mesh.new(getMeshData("data/models/ant_coach.ant2",0.5,"data/textures/models/ant_coach.png"),AGVector4.new(0,0,0),-30)
		#mesh=Mesh.new(getMeshData("data/models/mine.ant2",2),AGVector4.new(0,0,0),-30) # mine
		#setMesh(mesh)
		
	end
	
	def setupMesh
		mesh=Mesh.new(getMeshData("data/models/tower.ant2",3,"data/textures/models/tower_new.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end	
	
	
	def xmlName
		"antTower"
	end

	###############################
	# FLAG support
	###############################
	
	# override this for reseting the flag's position
	def setPos(p)
		super(p)
		if @flag then
			p=getPos3D
			p.y+=290
			p.z-=150
			@flag.setPos(p)
		end
	end
	
	def addFlag(owner)
		@flag=AntFlag.new
		setPos(getPos2D) # reset flag position
		$map.insertEntity(@flag)
	end


end