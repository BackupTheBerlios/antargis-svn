#!/usr/bin/env ruby

class AntTower<AntHouse
	def initialize
		super
		setProvide("tower",true)
	end
	
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/tower.ant2",3,"data/textures/models/tower_new.png"),AGVector4.new(0,0,0),-30)
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