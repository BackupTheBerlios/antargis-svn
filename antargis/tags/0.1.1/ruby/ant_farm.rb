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

class AntFarm<AntHouse
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
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/farm.ant2",0.2,"data/textures/models/farm.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

	def needed
		if resource.get("food")<30
			return "fruit"
		else
			return nil
		end
	end

	def xmlName
		"antFarm"
	end

	def process
		if resource.get("fruit")>0
			resource.add("food",1)
			resource.add("fruit",0)
		end
	end
end