
class AntTower<AntHouse
	def initialize
		super
		setProvide("tower",true)
		setMinimapColor(AGColor.new(0x22,0x22,0x22))
		setMinimapSize(6)

		@storeGood=["food","tool","corn","stone","wood"]
	end
	
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/tower.ant2",3,"data/textures/models/tower_new.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end	
	

	def neededStock
		{"fruit"=>15,"stone"=>15,"wood"=>15}
	end

	def resourceChanged
		puts "RESOURCE CHANGED"
		@storeGood.each{|r|
			setProvide(r,resource.get(r)>0)
		}
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

	def houseType
		"keep"
	end

	def process
		if resource.get("fruit")>0 and resource.get("food")<15
			amount=[resource.get("fruit"),3].min
			resource.add("food",amount)
			resource.sub("fruit",amount)
		end
	end
end