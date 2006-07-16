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
		setProvide("crop",true)
		@defeated=[]
		@atHome=[]
		@lastBirth=0
		@fields=[]
		setDirection(-30)
	end
	
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/farm.ant2",0.2,"data/textures/models/farm.png"),AGVector4.new(0,0,0),-30)
		#mesh=Mesh.new(getMap.getScene,getMeshData("data/models/townhall.ant2",3.2,"data/textures/models/townhall.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

	def neededStock
		{"fruit"=>30,"crop"=>30}
	end

	def assignJob(e)
		if @job.nil?
			if e.getMode=="makeField"
				pos=e.getPos2D
				f=AntField.new
				f.setPos(pos)
				getMap.insertEntity(f)
				puts "INSERT ENT #{@fields.length}"
				#getMap.endChange
				@fields.push(f)
				f.farm=self
				e.setMode("")
				e.newMoveJob(0,getPos2D,0)
				return
			elsif @fields.length<7
				e.setMode("makeField")
				angle=getRand*360
				radius=getRand*3+6
				e.newMoveJob(0,getPos2D+AGVector2.new(AGAngle.new(angle))*radius,0)
				return
			end
		end
		super
	end

	def removeField(f)
		@fields.delete(f)
		getMap.removeEntity(f)
		#getMap.endChange
	end

	def process
		if resource.get("fruit")>0
			amount=[resource.get("fruit"),3].min
			resource.add("food",amount)
			resource.sub("fruit",amount)
		end
	end
	def houseType
		"farm"
	end
end