#!/usr/bin/env ruby

def getFieldMeshData(size)
	if $fieldMeshes.nil?
		$fieldMeshes=[getMeshData("data/models/crop_small.ant2",2.2,"data/textures/models/crop_green.png"),
									getMeshData("data/models/crop_med.ant2",2.2,"data/textures/models/crop_yellow.png"),
									getMeshData("data/models/crop_high.ant2",2.2,"data/textures/models/crop_gold.png")]
		
	end
	$fieldMeshes[size] #getMeshData("data/models/ant_bakery.ant2",3.2,"data/textures/models/ant_bakery.png")
end


class AntField<AntRubyEntity
	def initialize
		super(AGVector2.new(0,0))
		setProvide("field",true)
		@size=(rand*3).to_i
		setupMesh
	end
	def resourceChanged
		setupMesh
	end
	
	
	private
	def setupMesh
		
		mesh=Mesh.new(getMap.getScene,getFieldMeshData(@size),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

end
