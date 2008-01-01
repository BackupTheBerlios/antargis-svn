#FIXME

class AntBakery<AntHouse
	def initialize(map)
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
		return if getScene.nil?
		setMesh
		p=AGVector3.new(0,1.6,2.2)
		addMesh(@smokeMesh=AntParticle.new(getMap.getScene,5),p)
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
		return if getScene.nil?
		if @smokeMesh
			if @smoke
				@smokeMesh.setEnabled((@smoke>0))
			else
				@smokeMesh.setEnabled(false)
			end
		end
	end

end