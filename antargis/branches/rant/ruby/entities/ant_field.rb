class AntField<AntRubyEntity
	attr_accessor :farm
	def initialize
		super(AGVector2.new(0,0))
		setProvide("field",true)
		@size=0
		#init fieldMeshes-var
		@max=AntModels.getMeshCount(:field)-1
		setupMesh
		@growTime=20
		@begin=true
		newRestJob(@growTime)
	end
	def resourceChanged
		if @size==@max
			if resource.get("crop")==0
				if @farm
					@farm.removeField(self)
					return
				end
			end
		end
		setupMesh
	end

	def eventNoJob
		@size=[@max,@size+1].min
		if @size==@max and @begin
			setProvide("crop",true)
			resource.set("crop",5)
			@begin=false
		end
		newRestJob(@growTime)
		resourceChanged
	end
	
	
	private
	def setupMesh
		setMesh(@size)
	end
end
