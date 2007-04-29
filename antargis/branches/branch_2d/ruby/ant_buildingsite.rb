
class AntBuildingSite<AntRubyEntity
	attr_accessor :building
	def initialize
		super(AGVector2.new(0,0))
		@progress=0
		#init fieldMeshes-var
		setupMesh
		newRestJob(300) # wait at least 5:00 minutes
		@doneSth=true
	end

	def incProgress(steps)
		@steps=steps
		o=@progress.to_i
		@progress+=(1.0/steps)*(@@buildingSiteMeshes.length-1)
		if o!=@progress.to_i
			setupMesh
		end
		@doneSth=true
	end
	def ready
		@progress>@@buildingSiteMeshes.length-1
	end

	# removes building site if nothing was done in some time
	def eventJobFinished
		if @doneSth
			newRestJob(30) # wait another 30 seconds
		else
			getMap.removeEntity(self) # remove building site
		end
		@doneSth=false
	end

	def getDescription
		a=_(building.to_s.gsub("Ant",""))
		d=_("This will be a {1}.",a)
		if @steps
			b=((@@buildingSiteMeshes.length-1)-@progress)*@steps/(@@buildingSiteMeshes.length-1)
			b=b.to_i.to_s
			d+=_("It will be ready after {1} more steps.",b)
		end
		d
	end

	private
	def setupMesh
		
		mesh=Mesh.new(getMap.getScene,AntBuildingSite.getBuildingSiteMeshData(@progress),AGVector4.new(0,0,0),-10)
		setMesh(mesh)
	end

	@@buildingSiteMeshes=nil
	def AntBuildingSite.getBuildingSiteMeshData(size)
		if @@buildingSiteMeshes.nil?
			@@buildingSiteMeshes=[
				getMeshData("data/models/building_site0.ant2",1.7,"data/textures/models/building_site0.png"),
				getMeshData("data/models/building_site1.ant2",1.7,"data/textures/models/building_site1.png"),
				getMeshData("data/models/building_site2.ant2",1.7,"data/textures/models/building_site1.png"),
				getMeshData("data/models/crop_high.ant2",2.2,"data/textures/models/crop_gold.png")
				]
			
		end
		size=[0,size.to_i,@@buildingSiteMeshes.length-1].sort[1]
		@@buildingSiteMeshes[size]
	end


end
