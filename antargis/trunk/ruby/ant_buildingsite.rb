#!/usr/bin/env ruby

def getBuildingSiteMeshData(size)
	if $buildingSiteMeshes.nil?
		$buildingSiteMeshes=[
	getMeshData("data/models/crop_tiny.ant2",2.2,"data/textures/models/crop_tiny.png"),
	getMeshData("data/models/crop_small.ant2",2.2,"data/textures/models/crop_green.png"),
	getMeshData("data/models/crop_med.ant2",2.2,"data/textures/models/crop_yellow.png"),
	getMeshData("data/models/crop_high.ant2",2.2,"data/textures/models/crop_gold.png")
	]
		
	end
	size=[0,size.to_i,$buildingSiteMeshes.length-1].sort[1]
	$buildingSiteMeshes[size]
end


class AntBuildingSite<AntRubyEntity
	def initialize
		super(AGVector2.new(0,0))
		@progress=0
		#init fieldMeshes-var
		setupMesh
	end

	def incProgress(steps)
		o=@progress.to_i
		@progress+=(1.0/steps)*($buildingSiteMeshes.length-1)
		if o!=@progress.to_i
			setupMesh
		end
	end
	def ready
		@progress>$buildingSiteMeshes.length-1
	end

	private
	def setupMesh
		
		mesh=Mesh.new(getMap.getScene,getBuildingSiteMeshData(@progress),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

end
