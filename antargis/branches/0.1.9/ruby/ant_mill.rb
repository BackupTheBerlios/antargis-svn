

class AntMill<AntHouse
	def initialize
		super
		@type=3
		setProvide("flour",true)
		@defeated=[]
		@atHome=[]
		@lastBirth=0
		setDirection(-30)
	end
	
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/ant_mill.ant2",3.2,"data/textures/models/ant_mill.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

	def neededStock
		{"food"=>20,"corn"=>30}
	end

	def process
		if resource.get("corn")>0 and resource.get("flour")<30
			amount=[resource.get("corn"),1].min
			resource.sub("corn",amount)
			resource.add("flour",amount*2)
		end
	end
	def houseType
		"mill"
	end
end