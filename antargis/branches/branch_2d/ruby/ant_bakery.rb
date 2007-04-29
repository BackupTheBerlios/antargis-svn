#FIXME

class AntBakery<AntHouse
	def initialize
		super
		@type=3
		setProvide("food",true)
		@defeated=[]
		@atHome=[]
		@lastBirth=0
		@smoke=0
		setDirection(-30)
	end
	
	def setupMesh
		setMesh(createModel(:bakery))
		#		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/ant_bakery.ant2",3.2,"data/textures/models/ant_bakery.png"),AGVector4.new(0,0,0),-30)
		#mesh=Mesh.new(getMap.getScene,getMeshData("data/models/townhall.ant2",3.2,"data/textures/models/townhall.png"),AGVector4.new(0,0,0),-30)
		#setMesh(mesh)
		p=AGVector3.new(0,1.6,2.2)
		addMesh(@smokeMesh=Smoke.new(getMap.getScene,5),p)
		checkSmoke
	end

	def neededStock
		{"food"=>15,"flour"=>15}
	end

	def process
		if resource.get("flour")>0 and resource.get("food")<60
			amount=[resource.get("flour"),1].min
			resource.sub("flour",amount)
			resource.add("food",amount*2)
			@smoke=1
			checkSmoke
		else
			@smoke=0
			checkSmoke
		end
	end
	def houseType
		"bakery"
	end
private
	# checks if smoke should be displayed
	def checkSmoke
		if @smokeMesh
			if @smoke
				@smokeMesh.setEnabled((@smoke>0))
			else
				@smokeMesh.setEnabled(false)
			end
		end
	end

end