def getBoatMeshData
	getMeshData("data/models/boat_simple.ant2",0.12,"data/textures/models/boat_simple.png")
end

# A boat lying around - for rowing this is not used!
class AntBoat<AntRubyEntity
	def initialize(p=AGVector2.new(0,0))
		super
		setupMesh
	end
private
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getBoatMeshData,AGVector4.new(0,0,0),0)
		setMesh(mesh)
	end
end