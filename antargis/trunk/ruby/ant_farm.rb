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

class AntHouseXXX


	def eventNoJob
		needed
		newRestJob(10)
	end
	
	def needed
		if @men.length==0
			tryGetMan
		end
	end
	
	def tryGetMan
		
	end

end

class AntFarm<AntHouse
	def initialize
		super
		@type=3
		setType("house")
		@defeated=[]
		@atHome=[]
		@lastBirth=0
		
		mesh=Mesh.new(getMeshData("data/models/farm.ant",2,"data/textures/models/tower_tex.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end


	def xmlName
		"antFarm"
	end
	
	
end