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
		@defeated=[]
		@atHome=[]
		setDirection(-30)
	end
	
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/townhall_try2.ant2",3.2,"data/textures/models/townhall_try2.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

	def neededStock
		{"food"=>30}
	end

	def houseType
		"townhall"
	end
end