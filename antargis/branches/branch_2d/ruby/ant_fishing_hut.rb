
class AntFishingHut<AntHouse
	def initialize
		super
		@type=3
		setProvide("food",true)
		@defeated=[]
		@atHome=[]
		@lastBirth=0
		setDirection(-30)
	end
	
	def setupMesh
		setMesh(createModel(:fishing_hut))
# 
# 
# 		data=getMeshData("data/models/fishing_hut.ant2",3.2,"data/textures/models/fishing_hut.png")
# 		data.setTransparent(true)
# 		mesh=Mesh.new(getMap.getScene,data,AGVector4.new(0,0,0),-30)
# 		setMesh(mesh)
	end

	def neededStock
		{"fish"=>20,"food"=>10}
	end

	def process
		if resource.get("fish")>0 and resource.get("food")<10
			amount=[resource.get("fish"),1].min
			resource.sub("fish",amount)
			resource.add("food",amount*2)
		end
	end
	def houseType
		"mill"
	end
end