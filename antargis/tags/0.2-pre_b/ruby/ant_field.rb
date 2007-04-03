class AntField<AntRubyEntity
	attr_accessor :farm
	def initialize
		super(AGVector2.new(0,0))
		setProvide("field",true)
		@size=0
		#init fieldMeshes-var
		AntField.getFieldMeshData(0)
		@max=@@fieldMeshes.length-1
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
		
		mesh=Mesh.new(getMap.getScene,AntField.getFieldMeshData(@size),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

	@@fieldMeshes=nil

	def AntField.getFieldMeshData(size)
		if @@fieldMeshes.nil?
			@@fieldMeshes=[
				getMeshData("data/models/crop_tiny.ant2",2.2,"data/textures/models/crop_tiny.png"),
				getMeshData("data/models/crop_small.ant2",2.2,"data/textures/models/crop_green.png"),
				getMeshData("data/models/crop_med.ant2",2.2,"data/textures/models/crop_yellow.png"),
				getMeshData("data/models/crop_high.ant2",2.2,"data/textures/models/crop_gold.png")]
		end
		@@fieldMeshes[size]
	end

end
