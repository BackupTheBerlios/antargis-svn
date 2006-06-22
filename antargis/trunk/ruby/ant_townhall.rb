#!/usr/bin/env ruby

# A farm produces food, uses men and tools
#
# so
#     needs at least: 1 man
#                     1 tool per man
#     at most:        10 man
#                     30 tools
#
# (A farm is connected to a village)
# A village is a virtual object, which provides:
#   - barn
#   - hotel / living house
#   - werkstatt

#
#
# food production is done through:
#    - get sheep
#    - (go fishing)
#    - go hunting (deer, wolf)
#    - fetch vegetables
#    - farming (wheat)

class AntTownHall<AntHouse
	def initialize
		super
		@type=3
		#setProvide("food",true)
		@defeated=[]
		@atHome=[]
		#@lastBirth=0
		setDirection(-30)
	end
	
	def setupMesh
		#mesh=Mesh.new(getMap.getScene,getMeshData("data/models/farm.ant2",0.2,"data/textures/models/farm.png"),AGVector4.new(0,0,0),-30)
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/townhall.ant2",3.2,"data/textures/models/townhall.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

	def needed
		if resource.get("food")<10
			return "fruit"
		else
			return nil
		end
	end

# 	def process
# 		if resource.get("fruit")>0
# 			amount=[resource.get("fruit"),3].min
# 			resource.add("food",amount)
# 			resource.sub("fruit",amount)
# 		end
# 	end
	def houseType
		"townhall"
	end
end