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
		setType("farm")
		@defeated=[]
		@atHome=[]
		@lastBirth=0
		
	end
	
	def setupMesh
		mesh=Mesh.new(getMeshData("data/models/farm.ant2",2,"data/textures/models/farm.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

	def needed
		if resource.get("food")<30
			return ["food","tree"]
		else
			return nil
		end
	end

	def xmlName
		"antFarm"
	end
	
	
end