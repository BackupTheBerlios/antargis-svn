#!/usr/bin/env ruby


#FIXME

class AntBakery<AntHouse
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
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/ant_bakery.ant2",3.2,"data/textures/models/ant_bakery.png"),AGVector4.new(0,0,0),-30)
		#mesh=Mesh.new(getMap.getScene,getMeshData("data/models/townhall.ant2",3.2,"data/textures/models/townhall.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

	def needed
		if resource.get("food")<30
			return "food"
		elsif resource.get("crop")<30
			return "crop"
		else
			return nil
		end
	end

	def process
		if resource.get("crop")>0
			amount=[resource.get("crop"),3].min
			resource.add("crop",amount)
			resource.sub("flour",amount)
		end
	end
	def houseType
		"mill"
	end
end